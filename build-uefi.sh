#!/usr/bin/env bash
#

set -e

platforms=('snb_ivb' 'hsw' 'byt' 'bdw' 'bsw' 'skl' 'apl' 'kbl' 'whl' 'glk' \
           'cml' 'jsl' 'tgl' 'adl' 'adl_n' 'str' 'pco' 'czn' 'mdn')
build_targets=()

json_file=cbmodels.json
rom_path=https://ethanthesleepy.one/public/mac_build/
echo -e "{" > $json_file

if [ -z "$1" ]; then
	for subdir in "${platforms[@]}"; do
		for cfg in configs/$subdir/config*.*; do
			build_targets+=("$(basename $cfg | cut -f2 -d'.')")
		done
	done
else
	build_targets=($@)
fi

mkdir -p macos/
failed_builds=macos/failed.log
touch $failed_builds

for device in "${build_targets[@]}"; do
	filename="coreboot_edk2-${device}-mrchromebox_$(date +"%Y%m%d")_macos.rom"
        rm -f macos/${filename}*
	rm -rf ./build
	cfg_file=$(find ./configs -name "config.$device.uefi")
	cp "$cfg_file" .config
	echo "CONFIG_LOCALVERSION=\"MrChromebox-4.21.0-macOS-v1\"" >> .config
	make clean
	make olddefconfig
        if  ! make -j$(nproc); then
                echo -e "${device}" >> $failed_builds
                continue
        fi
	cp ./build/coreboot.rom ./${filename}
	sha1sum ${filename} > ${filename}.sha1
	echo -e "\t\"${device}\": {" >> $json_file
	echo -e "\t\t\"url\": \"${rom_path}${filename}\"," >> $json_file
	echo -e "\t\t\"sha1\": \"$(cat ${filename}.sha1 | awk 'NR==1{print $1}')\"" >> $json_file
	echo -e "\t}," >> $json_file
        mv ${filename}* macos/
done
echo -e "}" >> $json_file

cp "${json_file}" macos/${json_file}
