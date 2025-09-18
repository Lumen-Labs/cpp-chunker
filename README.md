# Semantic Text Chunker

**Advanced semantic text chunking library that preserves meaning and context**

## Overview

This project provides a C++ library (with Python bindings via pybind11) for fast semantically chunking large bodies of text. It aims to split text into coherent, context-preserving segments using semantic similarity, discourse markers, and section detection, fast.

## Features

- Semantic chunking of text based on meaning and context
- Adjustable chunk size and coherence thresholds
- Extraction of chunk details: coherence scores, dominant topics, section types
- Python bindings for easy integration
- Memory-safe C++ implementation with comprehensive error handling

## Installation

### Prerequisites

- C++17 compiler (GCC 7+, Clang 5+, or MSVC 2017+)
- [pybind11](https://github.com/pybind/pybind11) (version 2.10.0+)
- CMake (version 3.16+)
- Python 3.7+

### Installing pybind11

#### Option 1: Using pip (Recommended)

```bash
pip install pybind11
```

#### Option 2: Using conda

```bash
conda install pybind11
```

#### Option 3: From source

```bash
git clone https://github.com/pybind/pybind11.git
cd pybind11
pip install .
```

#### Option 4: System package manager

```bash
# Ubuntu/Debian
sudo apt-get install pybind11-dev

# macOS with Homebrew
brew install pybind11

# Arch Linux
sudo pacman -S pybind11
```

### Build Instructions

#### Method 1: Using CMake (Recommended)

```bash
mkdir build
cd build
cmake ..
make
```

#### Method 2: Using pip (Automatic build)

```bash
pip install .
```

#### Method 3: Development installation

```bash
pip install -e .
```

## Usage

### Basic Usage

#### Simple Text Chunking

```python
import chunker_cpp

text = """This is a long text for testing the semantic chunker. It contains multiple sentences, some of which are quite lengthy and elaborate, while others are short. The purpose of this text is to simulate a realistic document that might be processed by the chunker.

In addition to regular sentences, this text includes various structures such as lists:
- First item in the list.
- Second item, which is a bit longer and more descriptive.
- Third item.

There are also paragraphs separated by blank lines.

Here is a new paragraph. It discusses a different topic and is intended to test how the chunker handles paragraph boundaries. Sometimes, paragraphs can be very long, spanning several lines and containing a lot of information. Other times, they are short.

Finally, this text ends with a concluding sentence to ensure the chunker can handle the end of input gracefully."""

# Basic chunking with default parameters
chunks = chunker_cpp.chunk_text_semantically(text)
print("Number of chunks:", len(chunks))
for i, chunk in enumerate(chunks):
    print(f"Chunk {i+1}: {chunk[:100]}...")
```

#### Advanced Usage with Custom Parameters

```python
# Custom chunking parameters
chunks = chunker_cpp.chunk_text_semantically(
    text,
    max_chunk_size=1000,      # Maximum characters per chunk
    min_chunk_size=200,       # Minimum characters per chunk
    min_coherence_threshold=0.5  # Higher threshold for more coherent chunks
)
```

#### Getting Detailed Chunk Information

```python
# Get detailed information about each chunk
chunk_details = chunker_cpp.get_chunk_details(text)

for i, detail in enumerate(chunk_details):
    print(f"Chunk {i+1}:")
    print(f"  Text: {detail['text'][:100]}...")
    print(f"  Coherence Score: {detail['coherence_score']:.3f}")
    print(f"  Dominant Topics: {detail['dominant_topics']}")
    print(f"  Sentence Count: {detail['sentence_count']}")
    print(f"  Section Type: {detail['primary_section_type']}")
    print()
```

### Using the Class Interface

```python
# Create a chunker instance
chunker = chunker_cpp.SemanticTextChunker()

# Use the instance methods
chunks = chunker.chunk_text_semantically(text, max_chunk_size=1500)
details = chunker.get_chunk_details(text, min_coherence_threshold=0.4)
```

### Error Handling and Memory Safety

The library includes comprehensive error handling and memory safety features:

```python
import chunker_cpp

# Safe handling of edge cases
try:
    # Empty string
    result = chunker_cpp.chunk_text_semantically("")
    print("Empty string handled:", result)

    # Very large text
    large_text = "This is a test sentence. " * 10000
    result = chunker_cpp.chunk_text_semantically(large_text, 1000, 200)
    print("Large text processed successfully")

    # Special characters
    special_text = "Text with special chars: \x00\x01\x02\n\r\t" + "A" * 1000
    result = chunker_cpp.chunk_text_semantically(special_text)
    print("Special characters handled safely")

except Exception as e:
    print(f"Error: {e}")
```

### Performance Considerations

```python
import time

# Benchmarking chunking performance
text = "Your large text here..." * 100  # Create a large text

start_time = time.time()
chunks = chunker_cpp.chunk_text_semantically(text)
end_time = time.time()

print(f"Processed {len(text)} characters in {end_time - start_time:.3f} seconds")
print(f"Generated {len(chunks)} chunks")
print(f"Average chunk size: {sum(len(chunk) for chunk in chunks) / len(chunks):.0f} characters")
```

## API Reference

### Functions

#### `chunk_text_semantically(text, max_chunk_size=2000, min_chunk_size=500, min_coherence_threshold=0.3)`

Chunk text semantically while preserving meaning and context.

**Parameters:**

- `text` (str): The input text to be chunked
- `max_chunk_size` (int): Maximum size for each chunk (default: 2000)
- `min_chunk_size` (int): Minimum size for each chunk (default: 500)
- `min_coherence_threshold` (float): Minimum coherence threshold (default: 0.3)

**Returns:**

- `List[str]`: List of text chunks

#### `get_chunk_details(text, max_chunk_size=2000, min_chunk_size=500, min_coherence_threshold=0.3)`

Get detailed information about each chunk including coherence scores and topics.

**Parameters:**

- `text` (str): The input text to be chunked
- `max_chunk_size` (int): Maximum size for each chunk (default: 2000)
- `min_chunk_size` (int): Minimum size for each chunk (default: 500)
- `min_coherence_threshold` (float): Minimum coherence threshold (default: 0.3)

**Returns:**

- `List[Dict[str, Any]]`: List of dictionaries containing chunk details

### Class

#### `SemanticTextChunker`

Advanced semantic text chunking class that preserves meaning and context.

**Methods:**

- `chunk_text_semantically(text, max_chunk_size=2000, min_chunk_size=500, min_coherence_threshold=0.3)`
- `get_chunk_details(text, max_chunk_size=2000, min_chunk_size=500, min_coherence_threshold=0.3)`

## Development

### Building from Source

```bash
# Clone the repository
git clone [<repository-url>](https://github.com/Lumen-Labs/cpp-chunker.git)
cd cpp_chunker

# Install dependencies
pip install pybind11

# Build using CMake
mkdir build && cd build
cmake ..
make

# Or build using pip
pip install .
```

### Running Tests

```bash
# Run basic functionality test
python test_cunk.py

# Run memory safety tests
python test_memory_safety.py
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.
