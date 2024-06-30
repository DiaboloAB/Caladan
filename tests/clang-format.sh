#!/bin/bash

# Function to display usage
usage() {
    echo "Usage: $0 <path>"
    echo "       path: the directory to check for format issues (e.g., src or include)"
    exit 1
}

# Check the number of arguments
if [ $# -ne 1 ]; then
    usage
fi

DIRECTORY=$1

# Check if the directory exists
if [ ! -d "$DIRECTORY" ]; then
    echo "Directory $DIRECTORY does not exist."
    exit 1
fi

# Initialize error flag
ERROR_FLAG=0

# Find all .cpp and .hpp files
FILES=$(find "$DIRECTORY" -type f \( -name '*.cpp' -o -name '*.hpp' \))

# Check each file
for FILE in $FILES; do
    # Run clang-format in dry run mode and capture any error
    clang-format --dry-run --Werror "$FILE"
    if [ $? -ne 0 ]; then
        echo "::error file=$FILE::File $FILE is not formatted correctly."
        ERROR_FLAG=1
    fi
done

# Exit with error flag status
exit $ERROR_FLAG
