#/bin/sh
set -e

if [ `uname` = "Darwin" ]; then

sudo xcode-select -s /Library/Developer/CommandLineTools
sudo gem install github-markup
sudo gem install commonmarker
sudo xcode-select -r

else

gem install github-markup
gem install commonmarker

fi
