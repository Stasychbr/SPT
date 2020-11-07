#include "Graph.h"
#include <limits>

Graph::Graph(uint size) {
    for (uint i = 0; i < size; i++) {
        _matrix.emplace_back(vector<uint>(size));
        for (auto& num : _matrix[i]) {
            num = UINT_MAX;
        }
    }
}

void Graph::setArc(uint head, uint tail, uint weight) {
    if (head < _matrix.size() && tail < _matrix.size()) {
        _matrix[tail][head] = weight;
    }
}

void Graph::print(FILE* file) {
    const int align = 10;
    fputc(' ', file);
    for (uint i = 1; i <= _matrix.size(); i++) {
        fprintf(file, "%*i", align, i);
    }
    for (uint i = 0; i < _matrix.size(); i++) {
        fprintf(file, "\n%i", i + 1);
        for (uint j = 0; j < _matrix.size(); j++) {
            if (_matrix[i][j] == UINT_MAX) {
                fprintf(file, "%*c", align, '-');
            }
            else {
                fprintf(file, "%*i", align, _matrix[i][j]);
            }
        }
    }
}
