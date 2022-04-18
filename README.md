# QuickSend

This package is meant to make it easy to send files from one system to
another without requiring any configuration or extra setup

## The Problem

There are millions of file transfer utilities out there and they all
suck, it's annoyingly hectic to get a file from one place to
another. Your options are:
- Send an email
- Upload/Download from a cloud
- Set up a samba server
- Set up an rsync daemon
- Set up an sshd server for scp
- Set up an FTP server
- Do it manually with a USB stick

It's too many hoops! All I want to do is basically "cp" from one
system to another with no hassle.

## The Solution

QuickSend is a simple program that doesn't try to do any more than it
should. All it does is transfer files, no configuration necessary.

To receive a file, run:
```bash
quicksend -r
```
Then on the sending machine
```bash
quicksend 192.168.1.xxx 12345 file_to_send
```
That is all. File transferred, hassle free (except for installing
quicksend), extremely tiny, simple, program that will take up nearly
no space. Currently compiles to 105kb on my system

