# OpenWrt HelloWorld Custom Feed

This repository contains a simple `helloworld` utility for OpenWrt.

## 1. Add the custom feed

From the OpenWrt source directory:

```sh
cd ~/openwrt
cp feeds.conf.default feeds.conf
nano feeds.conf
```

Add the following line to `feeds.conf`:

```sh
src-git myfeed git@github.com:Jaishankar872/openwrt-custom-feed.git;main
```

Update and inspect the feed:

```sh
./scripts/feeds update myfeed
ls feeds/myfeed/utils/helloworld/
```

## 2. Remove the previous package and temporary metadata

Before reinstalling or rebuilding, remove the old package from the OpenWrt
menu and source tree:

```sh
# Open menuconfig and remove Utilities -> HelloWorld if it is still selected.
make menuconfig
```

```sh
cd ~/openwrt

# Remove an older example package, if it exists.
rm -rf package/examples

# Remove the cached HelloWorld package metadata.
rm -f tmp/info/.packageinfo-examples_helloworld

# Search for any remaining HelloWorld package metadata.
find tmp/info -maxdepth 1 -type f -iname '*helloworld*' -ls
grep -R -n -w "helloworld" tmp/info/
```

## 3. Install the custom feed package

```sh
cd ~/openwrt

./scripts/feeds update myfeed
./scripts/feeds install -a -p myfeed

readlink -f package/feeds/myfeed/helloworld
```

Select the package in menuconfig:

```sh
make menuconfig
```

Enable as Module `M`:

```text
Utilities -> HelloWorld
```

## 4. Compile the package

Clean the previous build and compile the package:

```sh
cd ~/openwrt

make package/feeds/myfeed/helloworld/clean
make package/feeds/myfeed/helloworld/compile V=s
```

The package should be created in the matching package directory. The
current package version is `helloworld-1.1-r1.apk`:

```sh
find bin/packages -type f -name 'helloworld*.apk' -print
```

## 5. Copy and install on the OpenWrt device

Replace the architecture and IP address as needed:

```sh
cd ~/openwrt

scp -O bin/packages/aarch64_cortex-a53/myfeed/helloworld-1.1-r1.apk root@192.168.42.1:/tmp/
```

Connect to the router and replace the old package, if installed:

```sh
apk del helloworld
apk add --allow-untrusted /tmp/helloworld-1.1-r1.apk
```

Run the application:

```sh
helloworld
```
