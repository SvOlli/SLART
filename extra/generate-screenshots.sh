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

send Innuendo SHT MainWidget   "${dir}/Innuendo-Main.png"
send Innuendo SHT ConfigDialog "${dir}/Innuendo-Config.png"

send Karmadrome SHT MainWidget   "${dir}/Karmadrome-Main.png"
send Karmadrome SHT ConfigDialog "${dir}/Karmadrome-Config.png"

send Rubberbandman SHT MainWidget   "${dir}/Rubberbandman-Main.png"
send Rubberbandman SHT ConfigDialog "${dir}/Rubberbandman-Config.png"

send Stripped SHT MainWidget   "${dir}/Stripped-Main.png"
send Stripped SHT ConfigDialog "${dir}/Stripped-Config.png"

send Funkytown SHT MainWidget   "${dir}/Funkytown-Main.png"
send Funkytown SHT ConfigDialog "${dir}/Funkytown-Config.png"

send Partyman P0T 0
send Partyman SHT MainWidget   "${dir}/Partyman-Main-Help.png"
send Partyman P0T 1
send Partyman SHT MainWidget   "${dir}/Partyman-Main-Info.png"
send Partyman P0T 2
send Partyman SHT MainWidget   "${dir}/Partyman-Main-Search.png"
send Partyman P0T 3
send Partyman SHT MainWidget   "${dir}/Partyman-Main-Browse.png"
send Partyman P0T 1
send Partyman SHT ConfigDialog "${dir}/Partyman-Config.png"

ls -l "${dir}"

