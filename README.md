# On Air

A little script to let you handle when your camera turns on and off.

## Usage (with example script)

```
./camera_status | ./example
```

This command will not exit, as the `camera_status` script is designed to run as a daemon. To fully exercise this, open and close an application like Photo Booth that uses your camera.

## Writing your own script

The `camera_status` script spits 1s and 0s out to STDOUT. If you write your own script (starting from the `example` script is a great way to get started), pipe `camera_status` to that script, and read those 1s and 0s from your own script. Each will be terminated by a line ending.
