`tmux-battery`
==============

Incredibly simple CLI to show the battery level in tmux. Mac specific.

![screenshot](https://cloud.githubusercontent.com/assets/291371/3788012/17d75966-1a4c-11e4-8c16-2a46bba2bd1b.png)

You can see the checkmark on the left of the date.

There are two versions of this script. In the root directory is a JavaScript version. The JS version was ported from a shell script because it was cleaner. The `c` directory has a version written in C using CoreFoundation and IOKit. This version is understandably much faster (~10 to ~100x faster). I use the C version in my tmux status bar now.

## Usage
`tmux-battery`

It outputs:
- Full battery, plugged in: a checkmark
- Charging battery, plugged in: an up arrow and the percentage
- Discharging battery, not plugged in: a down arrow and the percentage
- Below the threshold (10% by default): a down arrow, the percentage, and an exclamation point
