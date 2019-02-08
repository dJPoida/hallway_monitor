# Clean the old data
rm -rf ../hallway_monitor/data/*

# Create some basic folders required for the copy
mkdir ../hallway_monitor/data

# Copy the files to the data directory
cp -r dist/* ../hallway_monitor/data 2>/dev/null