# On-Air

A little script to let you handle when your camera turns on and off.

## Usage (with example script)

```
./camera_status | ./example
```

This command will not exit, as the `camera_status` script is designed to run as a daemon. To fully exercise this, open and close an application like Photo Booth that uses your camera.

## Writing your own script

The `camera_status` script spits 1s and 0s out to STDOUT. If you write your own script (starting from the `example` script is a great way to get started), pipe `camera_status` to that script, and read those 1s and 0s from your own script. Each will be terminated by a line ending.

## Launchd support

To add On-Air to launchd for your machine, run the `generate` script with the name (or relative path) of your custom script, then add the filled-in `launchd.plist` as normal. For example, to integrate the example script with launchd, run:

```
$ ./generate example
$ launchctl load launchd.plist
```

That's it! At that point, the example script should be plugging away, writing
its example output to...`/dev/null`. (If you want to change that and see it in
action, just change the `Standard*Path` options in the generated plist.)

### Start on Login

To start On-Air whenever you log in, you'll need to add the same plist to `~/Library/LaunchAgents`. From the `on-air` directory:

```
ln -sf $(pwd)/launchd.plist ~/Library/LaunchAgents/com.schoonology.on-air.plist
```
