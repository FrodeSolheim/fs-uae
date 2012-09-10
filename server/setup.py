from distutils.core import setup

setup(name="fs_uae_netplay_server",
      version="1.3.26u1",
      description="FS-UAE Netplay Server",
      author="Frode Solheim",
      author_email="fs-uae@fengestad.no",
      url="http://www.fengestad.no/fs-uae",
      packages=["fs_uae_netplay_server"],
      scripts=["scripts/fs-uae-netplay-server",
               "scripts/fs-uae-game-server"],
)
