#!/bin/sh

OPK_NAME=griels.opk
echo ${OPK_NAME}

# create default.gcw0.desktop
cat > default.gcw0.desktop <<EOF
[Desktop Entry]
Name=Griel's Quest
Comment=Labyrinth puzzle
Exec=griels
Terminal=false
Type=Application
StartupNotify=true
Icon=png/icon_32x32
Categories=games;
EOF

# create opk
FLIST="data"
FLIST="${FLIST} fx"
FLIST="${FLIST} music"
FLIST="${FLIST} png"
FLIST="${FLIST} README"
FLIST="${FLIST} griels"
FLIST="${FLIST} default.gcw0.desktop"

rm -f ${OPK_NAME}
mksquashfs ${FLIST} ${OPK_NAME} -all-root -no-xattrs -noappend -no-exports

cat default.gcw0.desktop
rm -f default.gcw0.desktop