#!/bin/bash
# Profile a raytracer scene with Valgrind callgrind
# Usage: ./profile_test.sh <scene.json> <output_name>

if [ $# -lt 2 ]; then
    echo "Usage: ./profile_test.sh <scene.json> <output_name>"
    echo ""
    echo "Example:"
    echo "  ./profile_test.sh ../scenes/two-spheres-on-plane.json simple_scene"
    exit 1
fi

SCENE=$1
OUTPUT_NAME=$2
CALLGRIND_FILE="../tests/profiling/${OUTPUT_NAME}.callgrind"
IMAGE_FILE="../tests/profiling/${OUTPUT_NAME}.png"
PROFILE_GRAPH="../tests/profiling/${OUTPUT_NAME}_profile.png"

echo "============================================"
echo "Profiling Raytracer with Valgrind"
echo "============================================"
echo "Scene:        $SCENE"
echo "Output:       $OUTPUT_NAME"
echo ""

# Ensure profiling directory exists
mkdir -p ../tests/profiling

# Run with callgrind
echo "Running callgrind (this may take a while)..."
valgrind --tool=callgrind \
         --callgrind-out-file="$CALLGRIND_FILE" \
         ./raytracer "$SCENE" "$IMAGE_FILE"

if [ $? -ne 0 ]; then
    echo "❌ Callgrind failed!"
    exit 1
fi

echo ""
echo "✅ Callgrind complete"
echo "Callgrind data: $CALLGRIND_FILE"
echo ""

# Convert to visualization
echo "Generating call graph visualization..."
if command -v gprof2dot &> /dev/null; then
    gprof2dot -f callgrind "$CALLGRIND_FILE" | \
        dot -Tpng -o "$PROFILE_GRAPH"
    
    if [ $? -eq 0 ]; then
        echo "✅ Profile graph saved to: $PROFILE_GRAPH"
    else
        echo "⚠️  Failed to generate graph (dot command failed)"
    fi
else
    echo "⚠️  gprof2dot not found, skipping visualization"
    echo "Install with: pip3 install gprof2dot"
fi

echo ""
echo "============================================"
echo "Profiling complete!"
echo "============================================"
