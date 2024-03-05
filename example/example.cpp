#include "gnuplot.hpp"
#include <cmath>

int main()
{
    gnuplot<double> gp;
    gnuplotData<double> dataToPlot;

    // Set axis labels
    gp.sendLine("set ylabel \"Y\"");
    gp.sendLine("set xlabel \"X\"");

    // First dataset should be the x coordinates
    dataToPlot.data.push_back({"x", {}});

    // Create as much datasets as needed
    dataToPlot.data.push_back({"sinx/x", {}});
    dataToPlot.data.push_back({"x", {}});

    // This is just an example, one can fill the dataset however it wants
    doubleSet &xdataset = dataToPlot.data.at(0).second;
    doubleSet &y1dataset = dataToPlot.data.at(1).second;
    doubleSet &y2dataset = dataToPlot.data.at(2).second;

    // Fill the dataset
    for (int i = -20; i <= 20; i++)
    {
        if(i == 0)
        {
            xdataset.push_back(0);
            y1dataset.push_back(1);
            y2dataset.push_back(0);
            continue;
        }

        xdataset.push_back(i);
        y1dataset.push_back(sin(i) / i);
        y2dataset.push_back(i);
    }

    // Plot the data
    gp.plot(dataToPlot);

    // This gives the same result as above
    // gp.setBuffer(dataToPlot);
    // gp.plot();
    // gp.saveBufferToFile("output.data");

    return 0;
}
