ARCH=$(uname -m)
URL=""

case "$ARCH" in
    "arm64")
        URL="https://github.com/AllayMC/AllayLauncher/releases/latest/download/allay-launcher-macosx-arm64-release"
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