# System Setup Guide [HPCA-Assignment-01]

This document provides instructions for setting up an Ubuntu system with essential tools and configurations.

## Prerequisites

Ensure you have administrative (sudo) access to your Ubuntu machine.

### Step 1: Update and Upgrade Ubuntu

First, update the package lists and upgrade the installed packages to their latest versions.

```bash
sudo apt update
sudo apt upgrade -y
```
### Step 2: Install Required Packages
Install the necessary development tools and utilities:
```bash
sudo apt install -y git make gcc linux-tools-common
```
- git: Version control system.
- make: Build automation tool.
- gcc: GNU Compiler Collection.
- perf: Performance analysis tool.
### Step 3: Configure Kernel Parameters
Next, we'll set the kernel parameters for hugepages and kernel paranoid

Set Kernel Paranoid
To set the kernel paranoid level to -1, run:

```bash
echo -1 | sudo tee /proc/sys/kernel/perf_event_paranoid
```
To make this change permanent, add the following line to /etc/sysctl.conf:

```bash
sudo sh -c 'echo "kernel.perf_event_paranoid = -1" >> /etc/sysctl.conf'
```
Then, apply the changes:

```bash
sudo sysctl -p
```
### Step 4: Configure Hugepages
To set the number of 2MB hugepages to 1024, run:

```bash
echo 1024 | sudo tee /proc/sys/vm/nr_hugepages
```
To make this change permanent, add the following line to /etc/sysctl.conf:

```bash
sudo sh -c 'echo "vm.nr_hugepages = 1024" >> /etc/sysctl.conf'
```
Then, apply the changes:
```bash
sudo sysctl -p
```
#### Conclusion
Your Ubuntu system is now set up with the necessary development tools and configurations. You can verify the installations by running:

```bash
git --version
perf --version
make --version
gcc --version
```
Feel free to reach out if you encounter any issues!

### Step 5: Make the .cpp files
Run the following command to build all .cpp files and produce .out files
```bash
make
```
Run the following command to clean(remove) all .out files and just keep .cpp files
```bash
make clean
```

Finally run all the files using (this may choke your system)
```
chmod +x run.sh
./run.sh
```
Better run one file at a time
```bash
./run.sh filename.out
```
