# Compile the SCSS
npm run --prefix ./src build-css

# Clean the old data
rm -rf hallway_monitor/data/*

# Create some basic folders required for the copy
mkdir hallway_monitor/data/css
mkdir hallway_monitor/data/img
mkdir hallway_monitor/data/js

# Copy the files to the data directory
cp src/* hallway_monitor/data 2>/dev/null
cp -r src/css/* hallway_monitor/data/css
cp -r src/img/* hallway_monitor/data/img
cp -r src/js/* hallway_monitor/data/js

# Remove a few files we don't want in the data dir
rm hallway_monitor/data/package.json
rm hallway_monitor/data/package-lock.json

echo "Hallway Monitor built and deployed OK."