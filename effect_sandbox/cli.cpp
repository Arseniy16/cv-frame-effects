#include "cli.h"

#include <sstream>

namespace cli {

ProgressBar::ProgressBar(std::ostream& str, int total, std::string desc, int width, char fg, char bg) : 
	stream_(str), desc_(desc), total_(total), width_(width), bg_(bg), fg_(fg), status_(0), closed_(false) 
{ }

ProgressBar::~ProgressBar()
{
    close();
}

void ProgressBar::update(const size_t count) 
{    
    status_ += count;
    
    draw();

    return;
}

void ProgressBar::draw() 
{
    std::ostringstream prefix;

    if (desc_ != "") 
        prefix << desc_ << "| ";

    prefix << status_ << "/" << total_;
    
    prefix << " [";

    std::string prefixStr = prefix.str();

    stream_ << prefixStr;

    size_t width = std::max(0, int(width_ - prefixStr.size() - 1));

    if (status_ * width / total_ > (status_-1) * width / total_) 
    {
        for (int i = 0; i < (status_ * width) / total_; ++i) 
            stream_ << fg_;

        for (int i = (status_ * width) / total_; i < width; ++i) 
            stream_ << bg_;

        stream_ << "]";
    }

    stream_ << "\r";
    stream_.flush();

    return;
}

void ProgressBar::close() 
{    
    if (!closed_) 
    {
        stream_ << std::endl;
        closed_ = true;
    }

    return;
}


} // namespace cli


