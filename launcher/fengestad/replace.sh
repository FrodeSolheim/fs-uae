perl -e "s/no_fengestad_input/fengestad.input/g;" -pi $(find . -name "*.py")
perl -e "s/no_fengestad_gamecenter/fengestad.gamecenter/g;" -pi $(find . -name "*.py")
perl -e "s/gamecenter_glui/gamecenter.glui/g;" -pi $(find . -name "*.py")
perl -e "s/fengestad.fengestad./fengestad./g;" -pi $(find . -name "*.py")
