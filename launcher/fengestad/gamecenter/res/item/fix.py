import os
import Image
for item in os.listdir('.'):
  if not item.endswith('.png'): continue
  im = Image.open(item)
  if im.mode != "RGB":
    im = im.convert("RGB")
    im.save(item)

