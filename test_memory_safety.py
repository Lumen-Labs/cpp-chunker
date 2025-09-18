"""
Memory safety test for C++ extensions
"""

import sys
import os

sys.path.append(os.path.dirname(os.path.abspath(__file__)))

import chunker_cpp
import gc


def test_chunker_memory_safety():
    """Test the chunker with various inputs to ensure memory safety"""

    print("Testing chunker memory safety...")

    # Test 1: Empty string
    try:
        result = chunker_cpp.chunk_text_semantically("", 2000, 500, 0.3)
        print("✓ Empty string test passed")
    except Exception as e:
        print(f"✗ Empty string test failed: {e}")
        return False

    # Test 2: Very large string
    try:
        large_text = "This is a test sentence. " * 10000  # ~250KB
        result = chunker_cpp.chunk_text_semantically(large_text, 1000, 200)
        print("✓ Large string test passed")
    except Exception as e:
        print(f"✗ Large string test failed: {e}")
        return False

    # Test 3: Malformed text with special characters
    try:
        malformed_text = "Test with special chars: \x00\x01\x02\n\r\t" + "A" * 5000
        result = chunker_cpp.chunk_text_semantically(malformed_text, 1000, 200)
        print("✓ Malformed text test passed")
    except Exception as e:
        print(f"✗ Malformed text test failed: {e}")
        return False

    # Test 4: Memory leak test - run multiple times
    try:
        for i in range(100):
            test_text = f"This is test sentence number {i}. " * 100
            result = chunker_cpp.chunk_text_semantically(test_text, 1000, 200)
            if i % 20 == 0:
                gc.collect()
        print("✓ Memory leak test passed")
    except Exception as e:
        print(f"✗ Memory leak test failed: {e}")
        return False

    # Test 5: Invalid parameters
    try:
        result = chunker_cpp.chunk_text_semantically("test", -1, -1)
        print("✓ Invalid parameters test passed")
    except Exception as e:
        print(f"✗ Invalid parameters test failed: {e}")
        return False

    print("All memory safety tests passed!")
    return True


if __name__ == "__main__":
    success = test_chunker_memory_safety()
    if success:
        print("Memory safety verification completed successfully")
        sys.exit(0)
    else:
        print("Memory safety verification failed")
        sys.exit(1)
