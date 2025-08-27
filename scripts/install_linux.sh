ARCH=$(uname -m)
URL=""

case "$ARCH" in
    "x86_64")
        URL="https://github.com/AllayMC/AllayLauncher/releases/latest/download/allay-launcher-linux-x86_64-release"
        ;;
    "aarch64")
        URL="https://github.com/AllayMC/AllayLauncher/releases/latest/download/allay-launcher-linux-arm64-v8a-release"
        ;;
    "mips64")
        URL="https://github.com/AllayMC/AllayLauncher/releases/latest/download/allay-launcher-linux-mips64-release"
        ;;
    "mips64el")
        URL="https://github.com/AllayMC/AllayLauncher/releases/latest/download/allay-launcher-linux-mips64el-release"
        ;;
    *)
        echo "Unsupported architecture: $ARCH"
        exit 1
        ;;
esac

wget -O allay "$URL"
if [ $? -eq 0 ]; then
    chmod +x allay
else
    echo "Failed to download file. Please check your network."
    exit 1
fi

chmod +x allay
./allay