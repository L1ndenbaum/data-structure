class SparseMatrix
{
    private:
        class NonZeroElement
        {
            public:
                unsigned int row, column;
                int value;
        };
        unsigned int numRows, numColumns;
    public:
        SparseMatrix(unsigned int numRows, unsigned int numColumns, )
        {
            this->numRows = numRows;
            this->numColumns = numColumns;
        }
};