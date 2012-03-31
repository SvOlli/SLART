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

send SHT Innuendo Main "${dir}/Innuendo-Main.png"
send SHT Innuendo Config "${dir}/Innuendo-Config.png"

send SHT Karmadrome Main   "${dir}/Karmadrome-Main.png"
send SHT Karmadrome Config "${dir}/Karmadrome-Config.png"

send SHT Rubberbandman Main   "${dir}/Rubberbandman-Main.png"
send SHT Rubberbandman Config "${dir}/Rubberbandman-Config.png"

send SHT Stripped Main   "${dir}/Stripped-Main.png"
send SHT Stripped Config "${dir}/Stripped-Config.png"

send SHT Funkytown Main   "${dir}/Funkytown-Main.png"
send SHT Funkytown Config "${dir}/Funkytown-Config.png"

send P0T Help
send SHT Partyman Main   "${dir}/Partyman-Main-Help.png"
send SHT Partyman Help   "${dir}/Partyman-Help.png"
send P0T Info
send SHT Partyman Main   "${dir}/Partyman-Main-Info.png"
send SHT Partyman Info   "${dir}/Partyman-Info.png"
send P0T Search
send SHT Partyman Main   "${dir}/Partyman-Main-Search.png"
send SHT Partyman Search "${dir}/Partyman-Search.png"
send P0T Browse
send SHT Partyman Main   "${dir}/Partyman-Main-Browse.png"
send SHT Partyman Browse "${dir}/Partyman-Browse.png"
send P0T Player
send SHT Partyman Player "${dir}/Partyman-Player.png"
send P0T Info
send SHT Partyman Config "${dir}/Partyman-Config.png"

ls -l "${dir}"

