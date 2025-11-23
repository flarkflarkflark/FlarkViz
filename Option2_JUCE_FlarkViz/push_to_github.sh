#!/bin/bash
# FlarkViz GitHub Push Script
# Automates the process of pushing to GitHub

set -e  # Exit on error

REPO_URL="https://github.com/flarkflarkflark/FlarkViz.git"

echo "╔═══════════════════════════════════════════════════════╗"
echo "║         FlarkViz GitHub Push Automation               ║"
echo "║              by flarkAUDIO                            ║"
echo "╚═══════════════════════════════════════════════════════╝"
echo ""

# Check if we're in the FlarkViz directory
if [ ! -f "FlarkViz.jucer" ]; then
    echo "❌ Error: Not in FlarkViz directory!"
    echo "Please run this script from the FlarkViz root directory."
    exit 1
fi

echo "✅ Found FlarkViz project"
echo ""

# Step 1: Prepare README
echo "📝 Step 1: Preparing GitHub README..."
if [ -f "README_GITHUB.md" ]; then
    if [ -f "README.md" ]; then
        mv README.md README_DOCS.md
        echo "   Backed up README.md → README_DOCS.md"
    fi
    mv README_GITHUB.md README.md
    echo "   ✅ GitHub README ready"
else
    echo "   ⚠️  README_GITHUB.md not found, using existing README.md"
fi
echo ""

# Step 2: Initialize Git
echo "🔧 Step 2: Initializing Git repository..."
if [ -d ".git" ]; then
    echo "   Git already initialized"
else
    git init
    echo "   ✅ Git initialized"
fi

git branch -M main
echo "   ✅ Main branch set"
echo ""

# Step 3: Add remote
echo "🔗 Step 3: Adding remote repository..."
if git remote | grep -q "origin"; then
    echo "   Remote 'origin' already exists"
    git remote set-url origin "$REPO_URL"
    echo "   ✅ Updated remote URL"
else
    git remote add origin "$REPO_URL"
    echo "   ✅ Added remote: $REPO_URL"
fi
echo ""

# Step 4: Stage files
echo "📦 Step 4: Staging files..."
git add .gitignore LICENSE CONTRIBUTING.md
git add CMakeLists.txt FlarkViz.jucer
git add README.md BUILD.md QUICKSTART.md START_BUILDING.md
git add Source/
git add demo/*.cpp demo/*.h demo/Makefile 2>/dev/null || true

# Check status
STAGED_COUNT=$(git diff --cached --numstat | wc -l)
echo "   ✅ Staged $STAGED_COUNT files"
echo ""

# Step 5: Show what will be committed
echo "📋 Step 5: Files to be committed:"
git status --short | head -20
if [ $(git status --short | wc -l) -gt 20 ]; then
    echo "   ... and $(( $(git status --short | wc -l) - 20 )) more files"
fi
echo ""

# Step 6: Confirm
echo "❓ Ready to commit and push?"
echo "   Repository: $REPO_URL"
echo "   Branch: main"
echo ""
read -p "Continue? (y/n) " -n 1 -r
echo ""

if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "❌ Aborted"
    exit 1
fi

# Step 7: Commit
echo ""
echo "💾 Step 7: Creating commit..."
git commit -m "Initial commit: FlarkViz v0.1.0-alpha

- Complete JUCE project structure
- Audio analysis (FFT + beat detection)
- OpenGL rendering system
- MilkDrop3-style keyboard shortcuts
- Preset management framework
- Working demos (ASCII + verification)
- Comprehensive documentation

Part of flarkAUDIO ecosystem"

echo "   ✅ Commit created"
echo ""

# Step 8: Push
echo "🚀 Step 8: Pushing to GitHub..."
git push -u origin main

if [ $? -eq 0 ]; then
    echo "   ✅ Successfully pushed to GitHub!"
else
    echo "   ❌ Push failed. You may need to authenticate."
    echo "   Try running: gh auth login"
    exit 1
fi
echo ""

# Step 9: Create tag
echo "🏷️  Step 9: Creating release tag..."
git tag -a v0.1.0-alpha -m "FlarkViz v0.1.0-alpha - Initial release"
git push origin v0.1.0-alpha
echo "   ✅ Tag v0.1.0-alpha created and pushed"
echo ""

# Success!
echo "╔═══════════════════════════════════════════════════════╗"
echo "║              🎉 SUCCESS! 🎉                           ║"
echo "║                                                       ║"
echo "║  FlarkViz is now live on GitHub!                     ║"
echo "║                                                       ║"
echo "║  View it at:                                          ║"
echo "║  $REPO_URL"
echo "║                                                       ║"
echo "║  Next steps:                                          ║"
echo "║  1. Add repository description on GitHub             ║"
echo "║  2. Add topics (music-visualization, juce, etc.)     ║"
echo "║  3. Enable Issues and Discussions                    ║"
echo "║  4. Create first release (optional)                  ║"
echo "║  5. Share on social media!                           ║"
echo "╚═══════════════════════════════════════════════════════╝"
echo ""
