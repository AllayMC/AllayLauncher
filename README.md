# AllayLauncher

The launcher for [Allay](https://github.com/AllayMC/Allay) that aims to be small, fast and ready out of the box⚡

## Features

- [x] Detect java status
- [x] Manage Allay
- [x] Discover system proxy setting automatically
- [ ] Arm64 architecture support

## Installation

Install from one command line:

```bash
WIP
```

## Commands

Start the server (will download the latest nightly version of allay if allay haven't been downloaded):

```bash
allay
```

This is equal to:

```bash
allay -u -r -n
```

Available options:

- `-u` Check for update before starting the server
- `-r` Run allay server
- `-n` Use nightly build
- `-a` Pass arguments to java
- `-d` Use deamon mode, which the launcher will restart the server after server stopped

Start the server with specified memory limit:

Windows(cmd):

```bash
allay -u -r -n -a '-Xms1G -Xmx4G'
```

Linux(bash):

```bash
allay -u -r -n -a '\-Xms1G \-Xmx4G'
```

Start the server, and will restart the server if server crashed:

```bash
allay -u -r -n -d
```

Start the server, but do not check for update:

```bash
allay -r
```

## License

Copyright **:copyright: 2023-2024 AllayMC**, all rights reserved.

If not otherwise specified, project content is open source under the LGPL-3.0 license.
