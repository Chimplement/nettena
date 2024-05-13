# Nettena
Nettena is a small network sniffer which can log all network traffic coming into or out of your computer.
## Features
By default Nettena will log all incoming and outgoing packets on the stdout, including the source and destination ip addresses. Nettena will log the content of the packet as a hexdump. In the hexdump multiple lines of all zeros will be condensed into a single `*`. The hexdump can also be disabled with the `-h` flag.

Nettena also comes with a couple more useful options like only showing traffic from or to certain addresses, or limiting the amount of output lines per packet.
For a list of all options run:
```bash
./nettena -?
```

## Using
Nettena will work without providing any additional arguments like this:
```bash
./nettena
```
and it will keep waiting for incoming packets and print them in this format:
```
src: [source address] dst: [destination address]
[packet hex dump]
```
Keep in mind that the content also contains the ip header and the the transport layer's header.

### Building
Building is as simple as running make:
```bash
git clone https://github.com/Chimplement/nettena.git nettena
cd nettena
make
```
After which you could run it with sudo (which is not recommended) like this:
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
I made Nettena because I wanted to ease myself into raw socket programming because I am planning to make an arp-spoofer, and this seemed like a good complimentary project. But seeing your computers network traffic is also very interesting.