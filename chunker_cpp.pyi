"""
File: /chunker_cpp.pyi
Created Date: Thursday September 18th 2025
Author: Christian Nonis <alch.infoemail@gmail.com>
-----
Last Modified: Thursday September 18th 2025 9:38:02 pm
Modified By: the developer formerly known as Christian Nonis at <alch.infoemail@gmail.com>
-----
"""

from typing import List, Dict, Any, Union

class SemanticTextChunker:
    """Advanced semantic text chunking class that preserves meaning and context."""

    def __init__(self) -> None:
        """Initialize the semantic text chunker."""
        ...

    def chunk_text_semantically(
        self,
        text: str,
        max_chunk_size: int = 2000,
        min_chunk_size: int = 500,
        min_coherence_threshold: float = 0.3,
    ) -> List[str]:
        """
        Chunk text semantically while preserving meaning and context.

        Args:
            text: The input text to be chunked
            max_chunk_size: Maximum size for each chunk (default: 2000)
            min_chunk_size: Minimum size for each chunk (default: 500)
            min_coherence_threshold: Minimum coherence threshold (default: 0.3)

        Returns:
            List of text chunks
        """
        ...

    def get_chunk_details(
        self,
        text: str,
        max_chunk_size: int = 2000,
        min_chunk_size: int = 500,
        min_coherence_threshold: float = 0.3,
    ) -> List[Dict[str, Any]]:
        """
        Get detailed information about each chunk including coherence scores and topics.

        Args:
            text: The input text to be chunked
            max_chunk_size: Maximum size for each chunk (default: 2000)
            min_chunk_size: Minimum size for each chunk (default: 500)
            min_coherence_threshold: Minimum coherence threshold (default: 0.3)

        Returns:
            List of dictionaries containing chunk details
        """
        ...

def chunk_text_semantically(
    text: str,
    max_chunk_size: int = 2000,
    min_chunk_size: int = 500,
    min_coherence_threshold: float = 0.3,
) -> List[str]:
    """
    Chunk text semantically while preserving meaning and context (standalone function).

    Args:
        text: The input text to be chunked
        max_chunk_size: Maximum size for each chunk (default: 2000)
        min_chunk_size: Minimum size for each chunk (default: 500)
        min_coherence_threshold: Minimum coherence threshold (default: 0.3)

    Returns:
        List of text chunks
    """
    ...

def get_chunk_details(
    text: str,
    max_chunk_size: int = 2000,
    min_chunk_size: int = 500,
    min_coherence_threshold: float = 0.3,
) -> List[Dict[str, Any]]:
    """
    Get detailed information about each chunk including coherence scores and topics (standalone function).

    Args:
        text: The input text to be chunked
        max_chunk_size: Maximum size for each chunk (default: 2000)
        min_chunk_size: Minimum size for each chunk (default: 500)
        min_coherence_threshold: Minimum coherence threshold (default: 0.3)

    Returns:
        List of dictionaries containing chunk details
    """
    ...
