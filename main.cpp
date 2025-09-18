#include "chunker/chunker.h"

namespace py = pybind11;

PYBIND11_MODULE(chunker_cpp, m)
{
      m.doc() = "Advanced semantic text chunking library that preserves meaning and context";

      py::class_<SemanticTextChunker>(m, "SemanticTextChunker")
          .def(py::init<>())
          .def("chunk_text_semantically", &SemanticTextChunker::chunk_text_semantically,
               py::arg("text"),
               py::arg("max_chunk_size") = 2000,
               py::arg("min_chunk_size") = 500,
               py::arg("min_coherence_threshold") = 0.3,
               "Chunk text semantically while preserving meaning and context")
          .def("get_chunk_details", &SemanticTextChunker::get_chunk_details,
               py::arg("text"),
               py::arg("max_chunk_size") = 2000,
               py::arg("min_chunk_size") = 500,
               py::arg("min_coherence_threshold") = 0.3,
               "Get detailed information about each chunk including coherence scores and topics");

      m.def("chunk_text_semantically", &chunk_text_semantically_standalone,
            py::arg("text"),
            py::arg("max_chunk_size") = 2000,
            py::arg("min_chunk_size") = 500,
            py::arg("min_coherence_threshold") = 0.3,
            "Chunk text semantically while preserving meaning and context");

      m.def("get_chunk_details", &get_chunk_details_standalone,
            py::arg("text"),
            py::arg("max_chunk_size") = 2000,
            py::arg("min_chunk_size") = 500,
            py::arg("min_coherence_threshold") = 0.3,
            "Get detailed information about each chunk including coherence scores and topics");
}