#!/usr/bin/env python3

import sys
import urllib.request

# Constants
O3DS = 0
N3DS = 1

baseurl = "http://nus.cdn.c.shop.nintendowifi.net/ccs/download/"

cetkurls = [
            ["0004013800000002/cetk", "0004013820000002/cetk"],
            ["0004013800000102/cetk", "0004013820000102/cetk"],
            ["0004013800000202/cetk", "0004013820000202/cetk"]
           ]

firmurls = [
            ["0004013800000002/00000052", "0004013820000002/00000021"],
            ["0004013800000102/00000016", "0004013820000102/00000000"],
            ["0004013800000202/0000000B", "0004013820000202/00000000"]
           ]

intro = "FIRM Downloader\nDownloads the latest firmware files from the CDN"
help  = "Use either 'o3ds' or 'n3ds', depending on your console"

console = 0

# Actual code

if len(sys.argv) < 2:
    sys.exit(help)

if sys.argv[1] == "o3ds":
    console = O3DS
elif sys.argv[1] == "n3ds":
    console = N3DS
else: sys.exit("Unknown console: " + sys.argv[1])

print("Downloading NATIVE_FIRM")
urllib.request.urlretrieve(baseurl + firmurls[0][console], "firmware.bin")
urllib.request.urlretrieve(baseurl + cetkurls[0][console], "cetk")

print("Downloading TWL_FIRM")
urllib.request.urlretrieve(baseurl + firmurls[1][console], "twl_firmware.bin")
urllib.request.urlretrieve(baseurl + cetkurls[1][console], "twl_cetk")

print("Downloading AGB_FIRM")
urllib.request.urlretrieve(baseurl + firmurls[2][console], "agb_firmware.bin")
urllib.request.urlretrieve(baseurl + cetkurls[2][console], "agb_cetk")

exit()
