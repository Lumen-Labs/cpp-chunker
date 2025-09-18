import chunker_cpp
import sys

print("Python path:")
for path in sys.path:
    print(f"  {path}")

print("Module file:", chunker_cpp.__file__)
print("Available functions:", dir(chunker_cpp))
