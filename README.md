# pkt_summary

Used to showcase work during NASA Internship.

# What it does

Data that is transmitted from instruments is received in packet streams. This stream is received by Ground Support Equipment and stored in Telemetry files. This code is a CLI tool built on QT4 using a backported QT5 CLI parser. It reads TM files and ouputs the packet types and names found, as well as the amount of each packet discovered. Flags can be used to specify the input file and the packets (APIDs) to search for specifically.
