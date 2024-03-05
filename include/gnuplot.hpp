#pragma once

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

typedef std::vector<double> doubleSet;
typedef std::vector<int> intSet;

template <typename T>
struct gnuplotData
{
    std::vector<std::pair<std::string, std::vector<T>>> data{};
};

template <typename T>
class gnuplot
{
public:
    gnuplot()
    {
        std::cout << "Initializing gnuplot" << std::endl;
        pipe = popen("gnuplot -persist", "w");

        if (pipe)
            std::cout << "Success!" << std::endl;
        else
            std::cout << "Failed" << std::endl;
    }

    ~gnuplot()
    {
        if (pipe != NULL)
            pclose(pipe);
    }

    inline void sendLine(const char *command)
    {
        if (pipe == NULL)
        {
            std::cout << "Pipe not established" << std::endl;
            return;
        }

        // Send command to gnuplot
        fputs(command, pipe);
        fputs("\n", pipe);
    }

    inline void plot(const gnuplotData<T> &plotData = {})
    {
        // Check if pipe is open
        if (pipe == NULL)
        {
            std::cout << "Pipe not established" << std::endl;
            return;
        }

        gnuplotData<T> dataToPlot = plotData;
        // Check if we should use the buffer
        // If so, check if buffer is set
        if (plotData.data.empty())
        {
            if (buffer == nullptr)
            {
                std::cout << "Buffer not set" << std::endl;
                return;
            }

            dataToPlot = *buffer;
        }

        if (dataToPlot.data.size() < 2)
        {
            std::cout << "Incorrect dataset" << std::endl;
            return;
        }

        // Prepare the plot command
        std::string plotCommand = "plot";
        for (int i = 0; i < dataToPlot.data.size() - 1; i++)
        {
            plotCommand += " \'-\' u 1:2 title \"" + dataToPlot.data.at(i + 1).first + "\" w l,";
        }

        // Send plot command
        sendLine(plotCommand.c_str());

        // Send each dataset
        for (int i = 1; i < dataToPlot.data.size(); i++)
        {
            const std::vector<T> &xdataset = dataToPlot.data.at(0).second;
            const std::vector<T> &ydataset = dataToPlot.data.at(i).second;
            int iter = 0;
            for (const auto &xdata : xdataset)
            {
                sendLine((std::to_string(xdata) + " " + std::to_string(ydataset.at(iter))).c_str());
                iter++;
            }

            sendLine("e");
        }
    }

    inline void setBuffer(gnuplotData<T> &dataToBuffer)
    {
        if (dataToBuffer.data.empty())
            return;

        buffer = &dataToBuffer;
    }

    inline void saveBufferToFile(const char *fileName)
    {
        if (buffer == nullptr)
        {
            std::cout << "Buffer not set" << std::endl;
            return;
        }

        if (buffer->data.empty())
        {
            std::cout << "Buffer empty" << std::endl;
            return;
        }

        std::ofstream output(fileName, std::ios::trunc);
        if (!output)
        {
            std::cout << "Could not open the file" << std::endl;
            return;
        }

        int iter = 0;
        for (const auto &line : buffer->data.at(0).second)
        {
            output << line << " ";
            for (int i = 1; i < buffer->data.size(); i++)
            {
                output << buffer->data.at(i).second.at(iter) << " ";
            }
            output << "\n";
            iter++;
        }
    }

private:
    FILE *pipe;
    gnuplotData<T> *buffer;
};