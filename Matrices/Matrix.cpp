#include <vector>
using namespace std;

class Matrix
{   
    private:
        class Row // 设置代理类 以支持二维下标访问
        {
            private:
                double* rowData; // 每行的起始地址指针
            public:
                Row(double* rowAddr):rowData(rowAddr){}
                double& operator[](int column) { return rowData[column]; } // 返回一行中的某个元素
        };
    public:
        int numRows, numColumns;
        double* data;
        Matrix(int numRows, int numColumns):
            numRows(numRows), numColumns(numColumns), data(new double[numRows*numColumns]){}
        Row operator[](int row) { return Row(data + numColumns*row); } // 返回一行
};