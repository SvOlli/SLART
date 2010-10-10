#!/bin/sh

send()
{
echo $@
Innuendo $@
sleep 1
}

dir="/tmp/slart-screenshots"

rm -rf "${dir}"
mkdir -p "${dir}"

send SHT Innuendo MainWidget   "${dir}/Innuendo-Main.png"
send SHT Innuendo ConfigDialog "${dir}/Innuendo-Config.png"

send SHT Karmadrome MainWidget   "${dir}/Karmadrome-Main.png"
send SHT Karmadrome ConfigDialog "${dir}/Karmadrome-Config.png"

send SHT Rubberbandman MainWidget   "${dir}/Rubberbandman-Main.png"
send SHT Rubberbandman ConfigDialog "${dir}/Rubberbandman-Config.png"

send SHT Stripped MainWidget   "${dir}/Stripped-Main.png"
send SHT Stripped ConfigWidget "${dir}/Stripped-Config.png"

send SHT Funkytown MainWidget   "${dir}/Funkytown-Main.png"
send SHT Funkytown ConfigDialog "${dir}/Funkytown-Config.png"

send P0T 0
send SHT Partyman MainWidget   "${dir}/Partyman-Main-Help.png"
send P0T 1
send SHT Partyman MainWidget   "${dir}/Partyman-Main-Info.png"
send P0T 2
send SHT Partyman MainWidget   "${dir}/Partyman-Main-Search.png"
send P0T 3
send SHT Partyman MainWidget   "${dir}/Partyman-Main-Browse.png"
send P0T 1
send SHT Partyman ConfigDialog "${dir}/Partyman-Config.png"

ls -l "${dir}"

