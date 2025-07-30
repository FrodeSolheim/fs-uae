
#define PNG_SCREENSHOTS 1
#define IFF_SCREENSHOTS 2

#include <windows.h>

#include <stdio.h>

#include "sysconfig.h"
#include "sysdeps.h"
#include "options.h"
#include "custom.h"
#include "render.h"
#include "win32.h"
#include "win32gfx.h"
#include "direct3d.h"
#include "registry.h"
#include "gfxfilter.h"
#include "xwin.h"
#include "drawing.h"
#include "fsdb.h"
#include "zfile.h"
#include "gfxboard.h"

#include "png.h"

int screenshotmode = PNG_SCREENSHOTS;
int screenshot_originalsize = 0;
int screenshot_paletteindexed = 0;
int screenshot_clipmode = 0;
int screenshot_multi = 0;

extern bool need_genlock_data;
extern uae_u8 **row_map_genlock;

static bool usealpha(void)
{
	return need_genlock_data != 0 && row_map_genlock && currprefs.genlock_image && currprefs.genlock_alpha;
}

static void namesplit (TCHAR *s)
{
	int l;

	l = uaetcslen(s) - 1;
	while (l >= 0) {
		if (s[l] == '.')
			s[l] = 0;
		if (s[l] == '\\' || s[l] == '/' || s[l] == ':' || s[l] == '?') {
			l++;
			break;
		}
		l--;
	}
	if (l > 0)
		memmove (s, s + l, (uaetcslen(s + l) + 1) * sizeof (TCHAR));
}

static int toclipboard (BITMAPINFO *bi, void *bmp)
{
	struct AmigaMonitor *mon = &AMonitors[0];
	int v = 0;
	uae_u8 *dib = 0;
	HANDLE hg;

	if (!OpenClipboard(mon->hMainWnd))
		return v;
	EmptyClipboard ();
	hg = GlobalAlloc (GMEM_MOVEABLE | GMEM_DDESHARE, bi->bmiHeader.biSize + bi->bmiHeader.biSizeImage);
	if (hg) {
		dib = (uae_u8*)GlobalLock (hg);
		if (dib) {
			memcpy (dib, &bi->bmiHeader, bi->bmiHeader.biSize + bi->bmiHeader.biClrUsed * sizeof(RGBQUAD));
			memcpy (dib + bi->bmiHeader.biSize + +bi->bmiHeader.biClrUsed * sizeof(RGBQUAD), bmp, bi->bmiHeader.biSizeImage);
		}
		GlobalUnlock (hg);
		if (SetClipboardData (CF_DIB, hg))
			v = 1;
	}
	CloseClipboard ();
	if (!v)
		GlobalFree (hg);
	return v;
}

static HDC surface_dc, offscreen_dc;
static int surface_dc_monid;
static BITMAPINFO *bi; // bitmap info
static LPVOID lpvBits = NULL; // pointer to bitmap bits array
static HBITMAP offscreen_bitmap;
static int screenshot_prepared;
static int screenshot_multi_start;

void screenshot_free(void)
{
	if (surface_dc)
		releasehdc(surface_dc_monid, surface_dc);
	surface_dc = NULL;
	if(offscreen_dc)
		DeleteDC(offscreen_dc);
	offscreen_dc = NULL;
	if(offscreen_bitmap)
		DeleteObject(offscreen_bitmap);
	offscreen_bitmap = NULL;
	if(lpvBits)
		free(lpvBits);
	lpvBits = NULL;
	screenshot_prepared = FALSE;
}

static int rgb_rb, rgb_gb, rgb_bb, rgb_rs, rgb_gs, rgb_bs, rgb_ab, rgb_as;

static int screenshot_prepare(int monid, int imagemode, struct vidbuffer *vb, bool standard)
{
	struct AmigaMonitor *mon = &AMonitors[monid];
	int width, height;
	HGDIOBJ hgdiobj;
	int bits;
	int depth = usealpha() ? 32 : 24;
	bool renderTarget = true;
	bool locked = false;

	lockrtg();

	screenshot_free ();

	if (!standard) {
		regqueryint(NULL, _T("Screenshot_Original"), &screenshot_originalsize);
		regqueryint(NULL, _T("Screenshot_PaletteIndexed"), &screenshot_paletteindexed);
		regqueryint(NULL, _T("Screenshot_ClipMode"), &screenshot_clipmode);
		regqueryint(NULL, _T("Screenshot_Mode"), &screenshotmode);
	} else {
		screenshot_originalsize = 1;
		screenshot_clipmode = 0;
	}
	if (imagemode < 0)
		imagemode = screenshot_originalsize;

	if (imagemode) {
		int spitch, dpitch;
		uae_u8 *src, *dst, *mem;
		bool needfree = false;
		uae_u8 *palette = NULL;
		int rgb_bb2, rgb_gb2, rgb_rb2, rgb_ab2;
		int rgb_bs2, rgb_gs2, rgb_rs2, rgb_as2;
		uae_u8 pal[256 * 3];
		int screenshot_width = 0, screenshot_height = 0;
		int screenshot_xoffset = -1, screenshot_yoffset = -1;

		if (gfxboard_isgfxboardscreen(monid)) {
			src = mem = gfxboard_getrtgbuffer(monid, &width, &height, &spitch, &bits, pal);
			needfree = true;
			rgb_bb2 = 8;
			rgb_gb2 = 8;
			rgb_rb2 = 8;
			rgb_ab2 = 8;
			rgb_bs2 = 0;
			rgb_gs2 = 8;
			rgb_rs2 = 16;
			rgb_as2 = 24;
		} else if (vb) {
			width = vb->outwidth;
			height = vb->outheight;
			spitch = vb->rowbytes;
			bits = vb->pixbytes * 8;
			src = mem = vb->bufmem;
			rgb_bb2 = rgb_bb;
			rgb_gb2 = rgb_gb;
			rgb_rb2 = rgb_rb;
			rgb_ab2 = rgb_ab;
			rgb_bs2 = rgb_bs;
			rgb_gs2 = rgb_gs;
			rgb_rs2 = rgb_rs;
			rgb_as2 = rgb_as;
		} else {
			src = mem = getfilterbuffer(monid, &width, &height, &spitch, &bits, &locked);
			needfree = true;
			rgb_bb2 = rgb_bb;
			rgb_gb2 = rgb_gb;
			rgb_rb2 = rgb_rb;
			rgb_ab2 = rgb_ab;
			rgb_bs2 = rgb_bs;
			rgb_gs2 = rgb_gs;
			rgb_rs2 = rgb_rs;
			rgb_as2 = rgb_as;
		}
		if (src == NULL) {
			if (WIN32GFX_IsPicassoScreen(mon))
				renderTarget = false;
			goto donormal;
		}
		if (width == 0 || height == 0) {
			if (needfree) {
				if (WIN32GFX_IsPicassoScreen(mon))
					gfxboard_freertgbuffer(0, mem);
				else
					freefilterbuffer(0, mem, locked);
			}
			goto donormal;
		}

		int screenshot_xmult = 1;
		int screenshot_ymult = 1;

		screenshot_width = width;
		screenshot_height = height;
		if (!standard) {
			screenshot_xmult = currprefs.screenshot_xmult + 1;
			screenshot_ymult = currprefs.screenshot_ymult + 1;
			if (currprefs.screenshot_width > 0)
				screenshot_width = currprefs.screenshot_width;
			if (currprefs.screenshot_height > 0)
				screenshot_height = currprefs.screenshot_height;
			screenshot_xoffset = currprefs.screenshot_xoffset;
			screenshot_yoffset = currprefs.screenshot_yoffset;
		}

		if (!WIN32GFX_IsPicassoScreen(mon) && screenshot_clipmode == 1) {
			int cw, ch, cx, cy, crealh = 0, hres, vres;
			if (get_custom_limits(&cw, &ch, &cx, &cy, &crealh, &hres, &vres)) {
				int maxw = currprefs.screenshot_max_width << currprefs.gfx_resolution;
				int maxh = currprefs.screenshot_max_height << currprefs.gfx_vresolution;
				int minw = currprefs.screenshot_min_width << currprefs.gfx_resolution;
				int minh = currprefs.screenshot_min_height << currprefs.gfx_vresolution;
				if (minw > (maxhpos_display + 1) << currprefs.gfx_resolution)
					minw = (maxhpos_display + 1) << currprefs.gfx_resolution;
				if (minh > (maxvsize_display + 1) << currprefs.gfx_resolution)
					minh = (maxvsize_display + 1) << currprefs.gfx_resolution;
				if (maxw < minw)
					maxw = minw;
				if (maxh < minh)
					maxh = minh;
				screenshot_width = cw;
				screenshot_height = ch;
				screenshot_xoffset = cx;
				screenshot_yoffset = cy;
				if (screenshot_width < minw && minw > 0) {
					screenshot_xoffset -= (minw - screenshot_width) / 2;
					screenshot_width = minw;
				}
				if (screenshot_height < minh && minh > 0) {
					screenshot_yoffset -= (minh - screenshot_height) / 2;
					screenshot_height = minh;
				}
				if (screenshot_width > maxw && maxw > 0) {
					screenshot_xoffset += (screenshot_width - maxw) / 2;
					screenshot_width = maxw;
				}
				if (screenshot_height > maxh && maxh > 0) {
					screenshot_yoffset += (screenshot_height - maxh) / 2;
					screenshot_height = maxh;
				}
			}
		}

		if (screenshot_xmult < 1)
			screenshot_xmult = 1;
		if (screenshot_ymult < 1)
			screenshot_ymult = 1;

		int maxw_output = standard ? 0 : currprefs.screenshot_output_width;
		int maxh_output = standard ? 0 : currprefs.screenshot_output_height;
		while (maxw_output > screenshot_width * screenshot_xmult && screenshot_xmult < 8) {
			screenshot_xmult++;
		}
		while (maxh_output > screenshot_height * screenshot_ymult && screenshot_ymult < 8) {
			screenshot_ymult++;
		}

		int xoffset = screenshot_xoffset < 0 ? (screenshot_width - width) / 2 : -screenshot_xoffset;
		int yoffset = screenshot_yoffset < 0 ? (screenshot_height - height) / 2 : -screenshot_yoffset;

		ZeroMemory (bi, sizeof(bi));
		bi->bmiHeader.biClrUsed = bits <= 8 ? (1 << bits) : 0;
		bi->bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
		bi->bmiHeader.biWidth = screenshot_width * screenshot_xmult;
		bi->bmiHeader.biHeight = screenshot_height * screenshot_ymult;
		bi->bmiHeader.biPlanes = 1;
		bi->bmiHeader.biBitCount = bits <= 8 ? 8 : depth;
		bi->bmiHeader.biCompression = BI_RGB;
		dpitch = ((bi->bmiHeader.biWidth * bi->bmiHeader.biBitCount + 31) & ~31) / 8;
		bi->bmiHeader.biSizeImage = dpitch * bi->bmiHeader.biHeight;
		bi->bmiHeader.biXPelsPerMeter = 0;
		bi->bmiHeader.biYPelsPerMeter = 0;
		bi->bmiHeader.biClrImportant = 0;
		if (bits <= 8) {
			for (int i = 0; i < bi->bmiHeader.biClrUsed; i++) {
				bi->bmiColors[i].rgbRed = pal[i * 3  + 0];
				bi->bmiColors[i].rgbGreen = pal[i * 3  + 1];
				bi->bmiColors[i].rgbBlue = pal[i * 3  + 2];
			}
		}
		if (!(lpvBits = xmalloc(uae_u8, bi->bmiHeader.biSizeImage))) {
			if (needfree) {
				if (WIN32GFX_IsPicassoScreen(mon))
					gfxboard_freertgbuffer(monid, mem);
				else
					freefilterbuffer(monid, mem, locked);
			}
			goto oops;
		}

		int dpitch2 = dpitch;
		dpitch *= screenshot_ymult;

		dst = (uae_u8*)lpvBits;
		dst += dpitch * screenshot_height;
		if (yoffset > 0) {
			if (yoffset >= screenshot_height - height)
				yoffset = screenshot_height - height;
			dst -= dpitch * yoffset;
		} else if (yoffset < 0) {
			yoffset = -yoffset;
			if (yoffset >= height - screenshot_height)
				yoffset = height - screenshot_height;
			src += spitch * yoffset;
		}

		int xoffset2 = 0;
		if (xoffset < 0) {
			xoffset2 = -xoffset;
			xoffset = 0;
		}
		int dbpx = bits / 8;
		int sbpx = bits / 8;
		if (sbpx == 3)
			sbpx = 4;

		int xmult = screenshot_xmult;
		int ymult = screenshot_ymult;

		for (int y = 0; y < screenshot_height && y < height; y++) {
			uae_u8 *s, *d;
			dst -= dpitch;
			d = dst;
			s = src;
			if (xoffset > 0) {
				d += xoffset * dbpx;
			} else if (xoffset2 > 0) {
				s += xoffset2 * sbpx;
			}
			for (int x = 0; x < screenshot_width && x < width; x++) {
				int xx = x * screenshot_xmult;
				if (bits <= 8) {
					uae_u8 *d2 = d;
					for (int y2 = 0; y2 < ymult; y2++) {
						for (int x2 = 0; x2 < xmult; x2++) {
							d2[xx + x2] = s[x];
						}
						d2 += dpitch2;
					}
				} else {
					int shift;
					uae_u32 v = 0;
					uae_u32 v2, v2a, v2b, v2c, v2d;

					if (bits == 16)
						v = ((uae_u16*)s)[x];
					else if (bits == 32)
						v = ((uae_u32*)s)[x];

					shift = 8 - rgb_bb2;
					v2 = (v >> rgb_bs2) & ((1 << rgb_bb2) - 1);
					v2 <<= shift;
					if (rgb_bb2 < 8)
						v2 |= (v2 >> shift) & ((1 < shift) - 1);
					v2a = v2;

					shift = 8 - rgb_gb2;
					v2 = (v >> rgb_gs2) & ((1 << rgb_gb2) - 1);
					v2 <<= (8 - rgb_gb2);
					if (rgb_gb < 8)
						v2 |= (v2 >> shift) & ((1 < shift) - 1);
					v2b = v2;

					shift = 8 - rgb_rb2;
					v2 = (v >> rgb_rs2) & ((1 << rgb_rb2) - 1);
					v2 <<= (8 - rgb_rb2);
					if (rgb_rb < 8)
						v2 |= (v2 >> shift) & ((1 < shift) - 1);
					v2c = v2;

					if (depth == 32) {
						shift = 8 - rgb_ab2;
						v2 = (v >> rgb_as2) & ((1 << rgb_ab2) - 1);
						v2 <<= (8 - rgb_ab2);
						if (rgb_ab < 8)
							v2 |= (v2 >> shift) & ((1 < shift) - 1);
						v2d = v2;
					}

					uae_u8 *d2 = d;
					for (int y2 = 0; y2 < ymult; y2++) {
						for (int x2 = 0; x2 < xmult; x2++) {
							if (depth == 32) {
								d2[(xx + x2) * 4 + 0] = v2a;
								d2[(xx + x2) * 4 + 1] = v2b;
								d2[(xx + x2) * 4 + 2] = v2c;
								d2[(xx + x2) * 4 + 3] = v2d;
							} else {
								d2[(xx + x2) * 3 + 0] = v2a;
								d2[(xx + x2) * 3 + 1] = v2b;
								d2[(xx + x2) * 3 + 2] = v2c;
							}
						}
						d2 += dpitch2;
					}
				}
			}

			src += spitch;
		}
		if (needfree) {
			if (gfxboard_isgfxboardscreen(monid))
				gfxboard_freertgbuffer(monid, mem);
			else
				freefilterbuffer(monid, mem, locked);
		}

	} else {
donormal:
		bool d3dcaptured = false;
		width = WIN32GFX_GetWidth(mon);
		height = WIN32GFX_GetHeight(mon);
		if (!renderTarget && WIN32GFX_IsPicassoScreen(mon)) {
			struct picasso96_state_struct *state = &picasso96_state[monid];
			width = state->Width;
			height = state->Height;
		}
		if (D3D_isenabled(0) == 2) {
			int w, h, bits, pitch;
			void *data;
			bool got = D3D11_capture(monid, &data, &w, &h, &bits, &pitch, renderTarget);

			int dpitch = (((width * depth + 31) & ~31) / 8);
			lpvBits = xmalloc(uae_u8, dpitch * height);

			ZeroMemory(bi, sizeof(bi));
			bi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bi->bmiHeader.biWidth = width;
			bi->bmiHeader.biHeight = height;
			bi->bmiHeader.biPlanes = 1;
			bi->bmiHeader.biBitCount = depth;
			bi->bmiHeader.biCompression = BI_RGB;
			bi->bmiHeader.biSizeImage = dpitch * bi->bmiHeader.biHeight;
			bi->bmiHeader.biXPelsPerMeter = 0;
			bi->bmiHeader.biYPelsPerMeter = 0;
			bi->bmiHeader.biClrUsed = 0;
			bi->bmiHeader.biClrImportant = 0;

			if (got && lpvBits && bits <= 32) {
				for (int y = 0; y < h && y < height; y++) {
					uae_u8 *d = (uae_u8*)lpvBits + (height - y - 1) * dpitch;
					uae_u32 *s = (uae_u32*)((uae_u8*)data + y * pitch);
					for (int x = 0; x < w && x < width; x++) {
						uae_u32 v = *s++;
						d[0] = v >> 0;
						d[1] = v >> 8;
						d[2] = v >> 16;
						if (depth == 32) {
							d[3] = v >> 24;
							d += 4;
						} else {
							d += 3;
						}
					}
				}
			}
			if (got)
				D3D11_capture(monid, NULL, NULL, NULL, NULL, NULL, renderTarget);
			d3dcaptured = true;

		} else if (D3D_isenabled(0) == 1) {
			int w, h, bits;
			HRESULT hr;
			D3DLOCKED_RECT l;
			LPDIRECT3DSURFACE9 s = D3D_capture(monid, &w, &h, &bits, renderTarget);
			if (s) {
				hr = s->LockRect(&l, NULL, D3DLOCK_READONLY);
				if (SUCCEEDED(hr)) {
					int dpitch = (((width * depth + 31) & ~31) / 8);
					lpvBits = xmalloc(uae_u8, dpitch * height); 

					ZeroMemory(bi, sizeof(bi));
					bi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
					bi->bmiHeader.biWidth = width;
					bi->bmiHeader.biHeight = height;
					bi->bmiHeader.biPlanes = 1;
					bi->bmiHeader.biBitCount = depth;
					bi->bmiHeader.biCompression = BI_RGB;
					bi->bmiHeader.biSizeImage = dpitch * bi->bmiHeader.biHeight;
					bi->bmiHeader.biXPelsPerMeter = 0;
					bi->bmiHeader.biYPelsPerMeter = 0;
					bi->bmiHeader.biClrUsed = 0;
					bi->bmiHeader.biClrImportant = 0;

					if (lpvBits) {
						for (int y = 0; y < h && y < height; y++) {
							uae_u8 *d = (uae_u8*)lpvBits + (height - y - 1) * dpitch;
							uae_u32 *s = (uae_u32*)((uae_u8*)l.pBits + y * l.Pitch);
							for (int x = 0; x < w && x < width; x++) {
								uae_u32 v = *s++;
								d[0] = v >> 0;
								d[1] = v >> 8;
								d[2] = v >> 16;
								if (depth == 32) {
									d[3] = v >> 24;
									d += 4;
								} else {
									d += 3;
								}
							}
						}
					}
					s->UnlockRect();
					d3dcaptured = true;
				}
			}

		}
		if (!d3dcaptured) {
			surface_dc = gethdc(monid);
			surface_dc_monid = monid;
			if (surface_dc == NULL)
				goto oops;

			// need a HBITMAP to convert it to a DIB
			if ((offscreen_bitmap = CreateCompatibleBitmap (surface_dc, width, height)) == NULL)
				goto oops; // error

			// The bitmap is empty, so let's copy the contents of the surface to it.
			// For that we need to select it into a device context.
			if ((offscreen_dc = CreateCompatibleDC (surface_dc)) == NULL)
				goto oops; // error

			// select offscreen_bitmap into offscreen_dc
			hgdiobj = SelectObject (offscreen_dc, offscreen_bitmap);

			// now we can copy the contents of the surface to the offscreen bitmap
			BitBlt (offscreen_dc, 0, 0, width, height, surface_dc, 0, 0, SRCCOPY);

			// de-select offscreen_bitmap
			SelectObject (offscreen_dc, hgdiobj);

			ZeroMemory (bi, sizeof(bi));
			bi->bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
			bi->bmiHeader.biWidth = width;
			bi->bmiHeader.biHeight = height;
			bi->bmiHeader.biPlanes = 1;
			bi->bmiHeader.biBitCount = 24;
			bi->bmiHeader.biCompression = BI_RGB;
			bi->bmiHeader.biSizeImage = (((bi->bmiHeader.biWidth * bi->bmiHeader.biBitCount + 31) & ~31) / 8) * bi->bmiHeader.biHeight;
			bi->bmiHeader.biXPelsPerMeter = 0;
			bi->bmiHeader.biYPelsPerMeter = 0;
			bi->bmiHeader.biClrUsed = 0;
			bi->bmiHeader.biClrImportant = 0;

			// Reserve memory for bitmap bits
			if (!(lpvBits = xmalloc (uae_u8, bi->bmiHeader.biSizeImage)))
				goto oops; // out of memory

			// Have GetDIBits convert offscreen_bitmap to a DIB (device-independent bitmap):
			if (!GetDIBits (offscreen_dc, offscreen_bitmap, 0, bi->bmiHeader.biHeight, lpvBits, bi, DIB_RGB_COLORS))
				goto oops; // GetDIBits FAILED

			releasehdc(monid, surface_dc);
			surface_dc = NULL;
		}

		if (!lpvBits)
			goto oops;
	}
	unlockrtg();
	screenshot_prepared = TRUE;
	return 1;

oops:
	screenshot_free ();
	unlockrtg();
	return 0;
}

static int screenshot_prepare(int monid, struct vidbuffer *vb)
{
	return screenshot_prepare(monid, 1, vb, false);
}
int screenshot_prepare(int monid, int imagemode)
{
	return screenshot_prepare(monid, imagemode, NULL, false);
}
int screenshot_prepare(int monid)
{
	return screenshot_prepare(monid, -1);
}

void Screenshot_RGBinfo (int rb, int gb, int bb, int ab, int rs, int gs, int bs, int as)
{
	if (!bi)
		bi = xcalloc (BITMAPINFO, sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	rgb_rb = rb;
	rgb_gb = gb;
	rgb_bb = rb;
	rgb_ab = ab;
	rgb_rs = rs;
	rgb_gs = gs;
	rgb_bs = bs;
	rgb_as = as;
}

static uae_u32 uniquecolors[256] = { 0 };
static int uniquecolorcount, uniquecolordepth;
static uae_u8 *palettebm;

static void count_colors(bool alpha)
{
	int h = bi->bmiHeader.biHeight;
	int w = bi->bmiHeader.biWidth;
	int d = bi->bmiHeader.biBitCount;
	uae_u32 customcolors[256] = { 0 };
	bool uniquecolorsa[256] = { 0 };
	bool palettea[256] = { 0 };
	uae_u32 palette[256] = { 0 };
	uae_u8 indextab[256] = { 0 };
	int palettecount = 0;

	uniquecolorcount = 0;
	if (!screenshot_paletteindexed || alpha) {
		uniquecolorcount = -1;
		return;
	}
	if (d <= 8) {
		return;
	}
	palettebm = xcalloc(uae_u8, w * h);
	if (!palettebm) {
		return;
	}
	palettecount = 0;
	uae_u8 r, g, b;
	if (get_custom_color_reg(0, &r, &g, &b)) {
		palettea[palettecount] = true;
		palette[palettecount] = (b << 16) | (g << 8) | r;
		palettecount++;
	}
	for (int i = 0; i < h; i++) {
		uae_u8 *p = (uae_u8*)lpvBits + i * ((((w * 24) + 31) & ~31) / 8);
		for (int j = 0; j < w; j++) {
			uae_u32 co = (p[0] << 16) | (p[1] << 8) | (p[2] << 0);
			p += 3;
			int c = 0;
			if (palettecount >= 1 && co == palette[palettecount - 1]) {
				c = palettecount - 1;
			} else {
				for (c = 0; c < palettecount; c++) {
					if (palette[c] == co) {
						break;
					}
				}
			}
			if (c >= palettecount) {
				if (palettecount >= 256) {
					// run out of palette slots
					write_log("Run out of palette slots when counting colors.\n");
					uniquecolorcount = -1;
					xfree(palettebm);
					palettebm = NULL;
					return;
				}
				palettea[palettecount] = true;
				palette[palettecount] = co;
				palettecount++;
			}
		}
	}
	write_log("Screenshot color count: %d\n", palettecount);

	// get custom colors
	for (int i = 0; i < 256; i++) {
		uniquecolors[i] = 0;
		uniquecolorsa[i] = false;
	}
	int customcolorcnt = 0;
	for (int i = 0; i < 256; i++) {
		if (!get_custom_color_reg(i, &r, &g, &b))
			break;
		uae_u32 co = (b << 16) | (g << 8) | r;
		int j = 0;
		for (j = 0; j < customcolorcnt; j++) {
			if (uniquecolors[i] == co)
				break;
		}
		if (j >= customcolorcnt) {
			uniquecolors[i] = co;
			customcolorcnt++;
		}
	}
	// find matching colors from bitmap and allocate colors
	int match = 0;
	for (int i = 0; i < palettecount; i++) {
		if (palettea[i]) {
			uae_u32 cc = palette[i];
			for (int j = 0; j < customcolorcnt; j++) {
				uae_u32 cc2 = uniquecolors[j];
				if (!uniquecolorsa[j] && cc == cc2) {
					uniquecolorsa[j] = true;
					if (j >= uniquecolorcount) {
						uniquecolorcount = j + 1;
					}
					palettea[i] = false;
					match++;
					break;
				}
			}
		}
	}
	write_log("Screenshot custom register color matches: %d\n", match);
	// add remaining colors not yet matched
	match = 0;
	// use all unused colors if IFF mode to keep total colors as small as possible
	// if not iff: try to preserve first 32 colors.
	int safecolors = screenshotmode == 2 ? 0 : 32;
	if (uniquecolorcount < safecolors) {
		uniquecolorcount = safecolors;
	}
	for (int i = 0; i < palettecount; i++) {
		if (palettea[i]) {
			int j = 0;
			for (j = safecolors; j < 256 + safecolors; j++) {
				int jj = j & 255;
				if (!uniquecolorsa[jj]) {
					uniquecolors[jj] = palette[i];
					uniquecolorsa[jj] = true;
					palettea[i] = false;
					if (jj > uniquecolorcount) {
						uniquecolorcount = jj + 1;
					}
					match++;
					break;
				}
			}
			if (j >= 256 + safecolors) {
				// run out of palette slots
				write_log("Run out of palette slots when adding remaining colors.");
				uniquecolorcount = -1;
				xfree(palettebm);
				palettebm = NULL;
				return;
			}
		}
	}
	write_log("Screenshot non-custom register matched colors: %d\n", match);
	// create image
	int prevc = -1;
	for (int i = 0; i < h; i++) {
		uae_u8 *p = (uae_u8 *)lpvBits + i * ((((w * 24) + 31) & ~31) / 8);
		uae_u8 *dp = palettebm + w * i;
		for (int j = 0; j < w; j++) {
			uae_u32 co = (p[0] << 16) | (p[1] << 8) | (p[2] << 0);
			p += 3;
			int c = 0;
			if (prevc >= 0 && co == uniquecolors[prevc]) {
				c = prevc;
			} else {
				for (c = 0; c < 256; c++) {
					if (uniquecolors[c] == co) {
						prevc = c;
						if (c > uniquecolorcount) {
							uniquecolorcount = c + 1;
						}
						break;
					}
				}
			}
			*dp++ = (uae_u8)c;
		}
	}
	// select depth
	uniquecolordepth = 1;
	for (int i = 0; i < 8; i++) {
		if (uniquecolorcount > (1 << i)) {
			uniquecolordepth = i + 1;
		}
	}
}

#if PNG_SCREENSHOTS > 0

static void _cdecl pngtest_blah (png_structp png_ptr, png_const_charp message)
{
#if 1
	if (message) {
		TCHAR *msg = au(message);
		write_log (_T("libpng warning: '%s'\n"), msg);
		xfree (msg);
	}
#endif
}

static int savepng(FILE *fp, bool alpha)
{
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep *row_pointers;
	int h = bi->bmiHeader.biHeight;
	int w = bi->bmiHeader.biWidth;
	int d = bi->bmiHeader.biBitCount;
	png_color pngpal[256];
	int i;

	png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, pngtest_blah, pngtest_blah, pngtest_blah);
	if (!png_ptr)
		return 1;
	info_ptr = png_create_info_struct (png_ptr);
	if (!info_ptr) {
		png_destroy_write_struct (&png_ptr, NULL);
		return 2;
	}
	if (setjmp(png_jmpbuf (png_ptr))) {
		png_destroy_write_struct (&png_ptr, &info_ptr);
		return 3;
	}

	count_colors(alpha);
	png_init_io (png_ptr, fp);
	png_set_filter (png_ptr, 0, PNG_FILTER_NONE);
	png_set_IHDR (png_ptr, info_ptr,
		w, h, 8, uniquecolorcount <= 256 && uniquecolorcount >= 0 ? PNG_COLOR_TYPE_PALETTE : (alpha ? PNG_COLOR_TYPE_RGB_ALPHA : PNG_COLOR_TYPE_RGB),
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	row_pointers = xmalloc (png_bytep, h);
	if (palettebm) {
		for (i = 0; i < (1 << uniquecolordepth); i++) {
			pngpal[i].red = (uniquecolors[i] >> 0) & 0xff;
			pngpal[i].green = (uniquecolors[i] >> 8) & 0xff;
			pngpal[i].blue = (uniquecolors[i] >> 16) & 0xff;
		}
		png_set_PLTE(png_ptr, info_ptr, pngpal, 1 << uniquecolordepth);
		for (i = 0; i < h; i++) {
			int j = h - i - 1;
			row_pointers[i] = palettebm + j * w;
		}
	} else {
		if (d <= 8) {
			for (i = 0; i < (1 << d); i++) {
				pngpal[i].red = bi->bmiColors[i].rgbRed;
				pngpal[i].green = bi->bmiColors[i].rgbGreen;
				pngpal[i].blue = bi->bmiColors[i].rgbBlue;
			}
			png_set_PLTE(png_ptr, info_ptr, pngpal, 1 << d);
		}
		for (i = 0; i < h; i++) {
			int j = h - i - 1;
			row_pointers[i] = (uae_u8 *)lpvBits + j * (((w * (d <= 8 ? 8 : (alpha ? 32 : 24)) + 31) & ~31) / 8);
		}
	}
	png_set_rows (png_ptr, info_ptr, row_pointers);
	png_write_png (png_ptr,info_ptr, PNG_TRANSFORM_BGR, NULL);
	png_destroy_write_struct (&png_ptr, &info_ptr);
	xfree (row_pointers);
	xfree(palettebm);
	palettebm = NULL;
	return 0;
}

static void __cdecl write_data_fn(png_structp p, png_bytep data, png_size_t len)
{
	struct zfile *zf = (struct zfile*)png_get_io_ptr(p);
	zfile_fwrite(data, 1, len, zf);
}
static void __cdecl output_flush_fn(png_structp p)
{
}

static int saveiff(FILE *fp, bool alpha)
{
	const uae_u8 iffilbm[] = {
	'F','O','R','M',0,0,0,0,'I','L','B','M',
	'B','M','H','D',0,0,0,20, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	'C','A','M','G',0,0,0, 4,  0,0,0,0,
	};

	count_colors(alpha);

	int h = bi->bmiHeader.biHeight;
	int w = bi->bmiHeader.biWidth;
	int iffbpp = uniquecolordepth;
	if (uniquecolorcount < 0 || uniquecolorcount > 256) {
		iffbpp = 24;
	}

	int bodysize = (((w + 15) & ~15) / 8) * h * iffbpp;

	int iffsize = sizeof(iffilbm) + (8 + 256 * 3 + 1) + (4 + 4) + bodysize;
	uae_u8 *iff = xcalloc(uae_u8, iffsize);
	memcpy(iff, iffilbm, sizeof(iffilbm));
	if (!iff) {
		return 1;
	}
	uae_u8 *p = iff + 5 * 4;
	// BMHD
	p[0] = w >> 8;
	p[1] = w;
	p[2] = h >> 8;
	p[3] = h;
	p[8] = iffbpp;
	p[14] = 1;
	p[15] = 1;
	p[16] = w >> 8;
	p[17] = w;
	p[18] = h >> 8;
	p[19] = h;
	p = iff + sizeof iffilbm - 4;
	// CAMG
	if (w > 400)
		p[2] |= 0x80; // HIRES
	if (h > 300)
		p[3] |= 0x04; // LACE
	p += 4;
	if (iffbpp <= 8) {
		int cols = 1 << iffbpp;
		int cnt = 0;
		memcpy(p, "CMAP", 4);
		p[4] = 0;
		p[5] = 0;
		p[6] = (cols * 3) >> 8;
		p[7] = (cols * 3);
		p += 8;
		for (int i = 0; i < cols; i++) {
			*p++ = uniquecolors[i] >> 0;
			*p++ = uniquecolors[i] >> 8;
			*p++ = uniquecolors[i] >> 16;
			cnt += 3;
		}
		if (cnt & 1)
			*p++ = 0;
	}
	memcpy(p, "BODY", 4);
	p[4] = bodysize >> 24;
	p[5] = bodysize >> 16;
	p[6] = bodysize >> 8;
	p[7] = bodysize >> 0;
	p += 8;

	if (iffbpp <= 8) {
		for (int y = 0; y < h; y++) {
			uae_u8 *s = palettebm + ((h - 1) - y) * w;
			int b;
			for (b = 0; b < iffbpp; b++) {
				int mask2 = 1 << b;
				for (int x = 0; x < w; x++) {
					int off = x / 8;
					int mask = 1 << (7 - (x & 7));
					uae_u8 v = s[x]; 
					if (v & mask2)
						p[off] |= mask;
				}
				p += ((w + 15) & ~15) / 8;
			}
		}
	} else {
		for (int y = 0; y < h; y++) {
			uae_u32 *s = (uae_u32*)(((uae_u8*)lpvBits) + ((h - 1) - y) * ((w * (alpha ? 32 : 24) + 31) & ~31) / 8);
			int b, bb;
			for (bb = 0; bb < 3; bb++) {
				for (b = 0; b < 8; b++) {
					int mask2 = 1 << (((2 - bb) * 8) + b);
					for (int x = 0; x < w; x++) {
						int off = x / 8;
						int mask = 1 << (7 - (x & 7));
						uae_u32 v = s[x];
						if (v & mask2)
							p[off] |= mask;
					}
					p += ((w + 15) & ~15) / 8;
				}
			}
		}
	}

	int tsize = addrdiff(p, iff) - 8;
	p = iff + 4;
	p[0] = tsize >> 24;
	p[1] = tsize >> 16;
	p[2] = tsize >> 8;
	p[3] = tsize >> 0;
	fwrite(iff, 1, 8 + tsize + (tsize & 1), fp);
	xfree(iff);
	return 0;
}

static struct zfile *savepngzfile(bool alpha)
{
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep *row_pointers;
	int h = bi->bmiHeader.biHeight;
	int w = bi->bmiHeader.biWidth;
	int d = bi->bmiHeader.biBitCount;
	png_color pngpal[256];
	int i;
	struct zfile *zf;

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, pngtest_blah, pngtest_blah, pngtest_blah);
	if (!png_ptr)
		return NULL;
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_write_struct(&png_ptr, NULL);
		return NULL;
	}
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return NULL;
	}

	zf = zfile_fopen_empty(NULL, _T("screenshot"));
	if (!zf) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return NULL;
	}

	count_colors(alpha);
	png_set_write_fn(png_ptr, zf, write_data_fn, output_flush_fn);
	png_set_filter(png_ptr, 0, PNG_FILTER_NONE);
	png_set_IHDR(png_ptr, info_ptr,
		w, h, 8, uniquecolorcount <= 256 && uniquecolorcount >= 0 ? PNG_COLOR_TYPE_PALETTE : (alpha ? PNG_COLOR_TYPE_RGB_ALPHA : PNG_COLOR_TYPE_RGB),
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	if (d <= 8) {
		for (i = 0; i < (1 << d); i++) {
			pngpal[i].red = bi->bmiColors[i].rgbRed;
			pngpal[i].green = bi->bmiColors[i].rgbGreen;
			pngpal[i].blue = bi->bmiColors[i].rgbBlue;
		}
		png_set_PLTE(png_ptr, info_ptr, pngpal, 1 << d);
	}
	row_pointers = xmalloc(png_bytep, h);
	if (palettebm) {
		for (i = 0; i < (1 << uniquecolordepth); i++) {
			pngpal[i].red = (uniquecolors[i] >> 0) & 0xff;
			pngpal[i].green = (uniquecolors[i] >> 8) & 0xff;
			pngpal[i].blue = (uniquecolors[i] >> 16) & 0xff;
		}
		png_set_PLTE(png_ptr, info_ptr, pngpal, 1 << uniquecolordepth);
		for (i = 0; i < h; i++) {
			int j = h - i - 1;
			row_pointers[i] = palettebm + j * w;
		}
	} else {
		if (d <= 8) {
			for (i = 0; i < (1 << d); i++) {
				pngpal[i].red = bi->bmiColors[i].rgbRed;
				pngpal[i].green = bi->bmiColors[i].rgbGreen;
				pngpal[i].blue = bi->bmiColors[i].rgbBlue;
			}
			png_set_PLTE(png_ptr, info_ptr, pngpal, 1 << d);
		}
		for (i = 0; i < h; i++) {
			int j = h - i - 1;
			row_pointers[i] = (uae_u8 *)lpvBits + j * (((w * (d <= 8 ? 8 : (alpha ? 32 : 24)) + 31) & ~31) / 8);
		}
	}
	png_set_rows(png_ptr, info_ptr, row_pointers);
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_BGR, NULL);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	xfree(row_pointers);
	xfree(palettebm);
	palettebm = NULL;
	return zf;
}

#endif

static int savebmp (FILE *fp, bool alpha)
{
	BITMAPFILEHEADER bfh;
	// write the file header, bitmap information and pixel data
	bfh.bfType = 19778;
	bfh.bfSize = sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER) + (bi->bmiHeader.biClrUsed * sizeof(RGBQUAD)) + bi->bmiHeader.biSizeImage;
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER) + bi->bmiHeader.biClrUsed * sizeof(RGBQUAD);
	if (fwrite (&bfh, 1, sizeof (BITMAPFILEHEADER), fp) < sizeof (BITMAPFILEHEADER))
		return 1; // failed to write bitmap file header
	if (fwrite (bi, 1, sizeof (BITMAPINFOHEADER), fp) < sizeof (BITMAPINFOHEADER))
		return 2; // failed to write bitmap information header
	if (bi->bmiHeader.biClrUsed) {
		if (fwrite (bi->bmiColors, 1, bi->bmiHeader.biClrUsed * sizeof(RGBQUAD), fp) < bi->bmiHeader.biClrUsed * sizeof(RGBQUAD))
			return 3; // failed to write bitmap file header
	}
	if (fwrite (lpvBits, 1, bi->bmiHeader.biSizeImage, fp) < bi->bmiHeader.biSizeImage)
		return 4; // failed to write the bitmap
	return 0;
}

static void screenshot_prepare_multi(void)
{
	screenshot_multi_start = true;
}

static int filenumber = 0;
static int dirnumber = 1;

static void getscreenshotfilename(TCHAR *name)
{
	if (currprefs.floppyslots[0].dfxtype >= 0 && currprefs.floppyslots[0].df[0]) {
		_tcscpy(name, currprefs.floppyslots[0].df);
	} else if (currprefs.cdslots[0].inuse) {
		_tcscpy(name, currprefs.cdslots[0].name);
	}
}

/*
Captures the Amiga display (GDI, D3D) surface and saves it to file as a 24bit bitmap.
*/
int screenshotf(int monid, const TCHAR *spath, int mode, int doprepare, int imagemode, struct vidbuffer *vb)
{
	static int recursive;
	FILE *fp = NULL;
	int failed = 0;
	int screenshot_max = 1000; // limit 999 iterations / screenshots
	const TCHAR *format = _T("%s%s%s%03d.%s");
	bool alpha = usealpha();

	HBITMAP offscreen_bitmap = NULL; // bitmap that is converted to a DIB
	HDC offscreen_dc = NULL; // offscreen DC that we can select offscreen bitmap into

	if(recursive)
		return 0;

	recursive++;

	if (vb) {
		if (!screenshot_prepare(monid, vb))
			goto oops;
	} else if (!screenshot_prepared || doprepare) {	
		if (!screenshot_prepare(monid, imagemode))
			goto oops;
	}

	if (mode == 0) {
		toclipboard (bi, lpvBits);
	} else {
		TCHAR filename[MAX_DPATH];
		TCHAR path[MAX_DPATH];
		TCHAR name[MAX_DPATH];
		TCHAR underline[] = _T("_");

		if (spath) {
			fp = _tfopen (spath, _T("wb"));
			if (fp) {
#if PNG_SCREENSHOTS > 0
				if (screenshotmode == 1)
					failed = savepng (fp, alpha);
#endif
				if (screenshotmode == 2)
					failed = saveiff(fp, alpha);
				if (screenshotmode == 0)
					failed = savebmp (fp, alpha);
				fclose(fp);
				fp = NULL;
				if (failed)
					write_log(_T("Screenshot status %d ('%s')\n"), failed, spath);
				if (failed)
					_tunlink(spath);
			}
			goto oops;
		}
		fetch_path (_T("ScreenshotPath"), path, sizeof (path) / sizeof (TCHAR));
		CreateDirectory (path, NULL);
		if (screenshot_multi) {
			TCHAR *p = path + _tcslen(path);
			while(dirnumber < 1000) {
				_stprintf(p, _T("%03d\\"), dirnumber);
				if (!screenshot_multi_start)
					break;
				filenumber = 0;
				if (!my_existsdir(path) && !my_existsfile(path))
					break;
				dirnumber++;
			}
			format = _T("%s%s%s%05d.%s");
			screenshot_max = 100000;
			screenshot_multi_start = 0;
			if (dirnumber == 1000) {
				failed = 1;
				goto oops;
			}
			CreateDirectory(path, NULL);
		}

		name[0] = 0;
		getscreenshotfilename(name);
		if (!name[0])
			underline[0] = 0;
		namesplit (name);

		while(++filenumber < screenshot_max)
		{
			_stprintf (filename, format, path, name, underline, filenumber, screenshotmode == 2 ? _T("iff") : (screenshotmode ? _T("png") : _T("bmp")));
			if ((fp = _tfopen (filename, _T("rb"))) == NULL) // does file not exist?
			{
				int nok = 0;
				if ((fp = _tfopen (filename, _T("wb"))) == NULL) {
					write_log(_T("Screenshot error, can't open \"%s\" err=%d\n"), filename, GetLastError());
					goto oops; // error
				}
#if PNG_SCREENSHOTS > 0
				if (screenshotmode == 1)
					nok = savepng (fp, alpha);
#endif
				if (screenshotmode == 2)
					nok = saveiff(fp, alpha);
				if (screenshotmode == 0)
					nok = savebmp (fp, alpha);
				fclose(fp);
				if (nok && fp) {
					_tunlink(filename);
				}
				fp = NULL;
				if (nok) {
					write_log(_T("Screenshot error %d ('%s')\n"), nok, filename);
					goto oops;
				}
				write_log (_T("Screenshot saved as \"%s\"\n"), filename);
				failed = 0;
				break;
			}
			fclose (fp);
			fp = NULL;
		}
	}

oops:
	if(fp)
		fclose (fp);

	if (doprepare)
		screenshot_free ();

	recursive--;

	if (failed)
		screenshot_multi = 0;

	return failed == 0;
}

#include "drawing.h"

void screenshot(int monid, int mode, int doprepare)
{
	if (monid < 0) {
		monid = getfocusedmonitor();
	}

	if (mode == 2) {
		screenshot_multi = 10;
		screenshot_prepare_multi();
	} else if (mode == 3) {
		screenshot_multi = -1;
		screenshot_prepare_multi();
	} else if (mode == 4) {
		screenshot_multi = 0;
		filenumber = 0;
	} else {
		screenshotf(monid, NULL, mode, doprepare, -1, NULL);
	}
}

void screenshot_reset(void)
{
	screenshot_free();
}

uae_u8 *save_screenshot(int monid, size_t *len)
{
#if 0
	struct amigadisplay *ad = &adisplays[monid];
	if (ad->picasso_on)
		return NULL;
	struct vidbuf_description *avidinfo = &adisplays[monid].gfxvidinfo;
	struct vidbuffer vb;
	int w = avidinfo->drawbuffer.inwidth;
	int h = avidinfo->drawbuffer.inheight;
	if (!programmedmode) {
		h = (maxvpos + lof_display - minfirstline) << currprefs.gfx_vresolution;
	}
	if (interlace_seen && currprefs.gfx_vresolution > 0) {
		h -= 1 << (currprefs.gfx_vresolution - 1);
	}
	if (w > 0 && h > 0) {
		allocvidbuffer(monid, &vb, w, h, avidinfo->drawbuffer.pixbytes * 8);
		set_custom_limits(-1, -1, -1, -1);
		draw_frame(&vb);
		if (screenshot_prepare(monid, -1, &vb, true)) {
			struct zfile *zf = savepngzfile(false);
			uae_u8 *data = zfile_getdata(zf, 0, -1, len);
			zfile_fclose(zf);
			screenshot_free();
			return data;
		}
	}
#endif
	return NULL;
}
