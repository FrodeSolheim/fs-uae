#ifdef WITH_CEF

#ifdef __cplusplus
extern "C" {
#endif

int cef_init(int argc, char *argv[]);
void cef_destroy(void);
void cef_create_browser(void);

#ifdef __cplusplus
}
#endif

#endif /* WITH_CEF */
