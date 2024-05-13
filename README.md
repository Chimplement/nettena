# Nettena


## Features


## Using


### Building
Building is as simple as running make:
```bash
git clone https://github.com/Chimplement/nettena.git nettena
cd nettena
make
```
After which you could run it with sudo like this (which is not recommended):
```bash
sudo ./nettena
```
or, instead, run this once:
```bash
sudo setcap cap_net_admin,cap_net_raw=eip nettena
```
to give it the required capabilities only.
After which you can run it without sudo.

## Why
