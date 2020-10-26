#/bin/sh
set -e

if [ `uname` = "Darwin" ]; then

sudo xcode-select -s /Library/Developer/CommandLineTools
sudo gem install github-markup
sudo gem install commonmarker
sudo xcode-select -r

else

sudo apt install ruby ruby-dev
sudo gem install github-markup
sudo gem install commonmarker

fi
