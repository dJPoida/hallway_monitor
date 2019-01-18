# Compile the SCSS
npm run --prefix ./src build-css

# Clean the old data
rm -rf data/*

# Create some basic folders required for the copy
mkdir data/css
mkdir data/img
mkdir data/js

# Copy the files to the data directory
cp src/* data 2>/dev/null
cp -r src/css/* data/css
cp -r src/img/* data/img
cp -r src/js/* data/js

# Remove a few files we don't want in the data dir
rm data/package.json
rm data/package-lock.json

echo "Hallway Monitor built and deployed OK."