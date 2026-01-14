OUTPUT_DIR=JAN2026_group_10
SCLP=./reference-implementations/A0-sclp

rm -rf "$OUTPUT_DIR"
mkdir "$OUTPUT_DIR"

for f in example-programs/Level-5-test-cases/*.c; do
	"$SCLP" --show-ast "$f"
	"$SCLP" --show-tac "$f"
	"$SCLP" --show-rtl "$f"
	"$SCLP" --show-asm "$f"
	mv "$f.ast" "$f.tac" "$f.rtl" "$f.spim" "$OUTPUT_DIR"
done

for f in example-programs/Level-5-invalid-test-cases/*.c; do
	"$SCLP" --show-ast "$f"
	"$SCLP" --show-tac "$f"
	"$SCLP" --show-rtl "$f"
	"$SCLP" --show-asm "$f"
	mv "$f.ast" "$f.tac" "$f.rtl" "$f.spim" "$OUTPUT_DIR"
done
 
