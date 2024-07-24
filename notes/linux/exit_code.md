# exit code

The [posix standard](https://tldp.org/LDP/abs/html/exitcodes.html) says:

> exit codes 1 - 2, 126 - 165, and 255 [1] have special meanings, and should therefore be avoided for user-specified exit parameters.

and

> Out of range exit values can result in unexpected exit codes. An exit value greater than 255 returns an exit code modulo 256. For example, exit 3809 gives an exit code of 225 (3809 % 256 = 225).

If you are making something that could be turned into a service, it's good to avoid conflicts with (or reuse meaning from) [systemd's exit codes](https://freedesktop.org/software/systemd/man/systemd.exec.html#id-1.20.8) which defines code 2-7,200-242. This link also references BSD codes 64-78.

Therefore if you are not re-using a definition from one of these standards, I'd suggest codes

- 8-63,
- 79-125,
- 166-199, or
- 243-255