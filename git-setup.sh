#!/bin/bash
# Git Remote Setup Script
# Usage: ./git-setup.sh <repository-url>

if [ -z "$1" ]; then
    echo "Error: Repository URL required"
    echo ""
    echo "Usage: $0 <repository-url>"
    echo ""
    echo "Examples:"
    echo "  $0 git@github.com:yourusername/arboles.git"
    echo "  $0 https://github.com/yourusername/arboles.git"
    echo ""
    echo "Steps to create a new repository:"
    echo "  1. Go to https://github.com/new"
    echo "  2. Create repository named 'arboles'"
    echo "  3. Do NOT initialize with README, .gitignore, or license"
    echo "  4. Copy the repository URL"
    echo "  5. Run this script with the URL"
    exit 1
fi

REPO_URL="$1"

echo "Setting up remote repository..."
echo "URL: $REPO_URL"
echo ""

# Add remote origin
git remote add origin "$REPO_URL"

# Verify remote was added
echo "Remote configured:"
git remote -v
echo ""

# Rename branch to main (if you prefer)
read -p "Rename branch from 'master' to 'main'? (y/N) " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    git branch -M main
    echo "Branch renamed to 'main'"
else
    echo "Keeping branch name 'master'"
fi
echo ""

# Push to remote
echo "Ready to push to remote!"
echo "Run: git push -u origin $(git branch --show-current)"
echo ""
echo "Or run: git push -u origin $(git branch --show-current) --force"
echo "(Use --force only if you're sure the remote is empty)"
