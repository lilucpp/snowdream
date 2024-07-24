# **Git Installation and SSH Configuration**

> By: Sagar Chamling

## **Installation**

### *For Linux (Debian/Ubuntu):*

	sudo apt install git

We can check if **Git** is installed by typing `git --version` in your Terminal or Git Bash (Windows) respectively.

### *For Windows:*

Download Git for Windows from this link https://git-scm.com/downloads
    
## **Configuration**

### *Set up SSH for Git*

Open your Terminal or Git Bash(Windows).

* **Ensure we have SSH client installed:**

```shell
$ ssh -v
usage: ssh 	[-1246AaCfGgKkMNnqsTtVvXxYy] [-b bind_address] [-c cipher_spec]
[-D [bind_address:]port] [-E log_file] [-e escape_char]
[-F configfile] [-I pkcs11] [-i identity_file] [-L address]
[-l login_name] [-m mac_spec] [-O ctl_cmd] [-o option] [-p port]
[-Q query_option] [-R address] [-S ctl_path] [-W host:port]
[-w local_tun[:remote_tun]] [user@]hostname [command]
```
List the contents of your ~/.ssh directory.
```shell
$ ls -a ~/.ssh 
known_hosts
```
 *If we have already setup default identity, we can see two id_ files*
```shell
$ ls -a ~/.ssh
.  ..  id_rsa  id_rsa.pub  known_hosts
```

* **Set up our default identity:**

The system adds keys for all identities to the `/home/<yourname>/.ssh` (Linux) or `/c/Users/<yourname>/.ssh` (Windows) directory. The following procedure creates a default identity.

Hit `ssh-keygen` command on Terminal or Git Bash.

```shell
$ ssh-keygen 
Generating public/private rsa key pair.
Enter file in which to save the key (/c/Users/<yourname>/.ssh/id_rsa):
```
Press enter to accept the default key and location or we can set somewhere else.

We enter and re-enter a passphrase when prompted. Unless you need a key for a process such as script, you should always provide a passphrase. The command creates your default identity with its public and private keys

```shell
$ ssh-keygen
Generating public/private rsa key pair.
Enter file in which to save the key (/c/Users/<yourname>/.ssh/id_rsa):
Created directory '/c/<yourname>/<yourname>/.ssh'.
Enter passphrase (empty for no passphrase):
Enter same passphrase again:
Your identification has been saved in /c/Users/<yourname>/.ssh/id_rsa.
Your public key has been saved in /c/Users/<yourname>/.ssh/id_rsa.pub.
The key fingerprint is:
SHA256:+V6cLhFWngIFC+TdHwQHxJP39lsZUWnP6TDj1/7hBVA <yourname>@<yourname>
The key's randomart image is:
+---[RSA 2048]----+
|     .o .==+o E +|
|     . o.o+oo. + |
|      . o..=oo..+|
|         .+.o*ooo|
|        S. oo.*.+|
|         ... o =+|
|          ..+ .o+|
|         ..o  ..+|
|          ...  .o|
+----[SHA256]-----+
```

List the contents of your ~/.ssh directory.
```shell 
$ ls ~/.ssh 
.  ..  id_rsa  id_rsa.pub  known_hosts
```

* **Adding your SSH key to the ssh-agent:**

Ensure ssh-agent is enabled:
```shell
$ eval "$(ssh-agent -s)"
# Agent pid 59566
```
Add your SSH key to the ssh-agent
```shell
ssh-add ~/.ssh/id_rsa
```

* **Find and take a note of your public key fingerprint. If you're using OpenSSH 6.7 or older:**

```shell
$ ssh-add -l
# 2048 a0:dd:42:3c:5a:9d:e4:2a:21:52:4e:78:07:6e:c8:4d /Users/<yourname>/.ssh/id_rsa (RSA)

If you are using OpenSSH 6.8 or newer:

$ ssh-add -l -E md5
# 2048 MD5:a0:dd:42:3c:5a:9d:e4:2a:21:52:4e:78:07:6e:c8:4d /Users/<yourname>/.ssh/id_rsa (RSA)
```

* **Install the public key on your Remote GitHub/Bitbucket account:**

```shell
$ cat ~/.ssh/id_rsa.pub
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQC2zeZVIph1tP0UZJ007AC1OWqThpYjDlao1PlQnZbrSMeS8LXkU/nMxuZdAv+2JeqhezOtb6/e8e50NOTWB9Z2O8thCMwc29cp6C+vHL2oWQYMcCOuT34/R2yDEOMQ5nkIZ1fVFJNCTIZUaKjyaHX89w0v2p9cMsZ1q36w9lEdKXs8N5fuN/6rAy3JQgMcbD+dDd0cWpP8CLiUyNCq32xwqhX+nS1P43AgOQdLpX74uljwr7rE2CmrJQkvh/m+h68tv8+mLMGJtg5cJ+doZ+9r9yPhKJYGEsW4bL+8sSRQn3gJWUib8xhOgaWrMfXj+94o1KbcI12lK772GNyP74rX <yourname>@<yourname>
```

Copy this output to respective SSH keys setting.
* GitHub Setting >> SSH and GPG keys >> New SSH key
* Bitbucket Setting >> SSH keys >> Add Key

### *You are Done*
You have now successfully configured SSH for Git in Windows/Linux/Mac OS.

## **References**

* https://help.github.com/articles/connecting-to-github-with-ssh/
* https://confluence.atlassian.com/bitbucket/set-up-ssh-for-git-728138079.html