"""
File: /test_cunk.py
Created Date: Thursday September 18th 2025
Author: Christian Nonis <alch.infoemail@gmail.com>
-----
Last Modified: Thursday September 18th 2025 9:00:28 pm
Modified By: the developer formerly known as Christian Nonis at <alch.infoemail@gmail.com>
-----
"""

import chunker_cpp

print(
    chunker_cpp.chunk_text_semantically(
        """This is a long text for testing the semantic chunker. It contains multiple sentences, some of which are quite lengthy and elaborate, while others are short. The purpose of this text is to simulate a realistic document that might be processed by the chunker. 

        In addition to regular sentences, this text includes various structures such as lists:
        - First item in the list.
        - Second item, which is a bit longer and more descriptive.
        - Third item.

        There are also paragraphs separated by blank lines.

        Here is a new paragraph. It discusses a different topic and is intended to test how the chunker handles paragraph boundaries. Sometimes, paragraphs can be very long, spanning several lines and containing a lot of information. Other times, they are short.

        Finally, this text ends with a concluding sentence to ensure the chunker can handle the end of input gracefully.
        """,
        50,
        50,
        0.3,
    )
)
