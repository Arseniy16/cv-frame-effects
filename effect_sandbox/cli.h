#ifndef _CLI_H_
#define _CLI_H_

#include <iostream>

namespace cli {

class ProgressBar 
{
public:
    ProgressBar(std::ostream& str, int total = -1, std::string desc = "",
                int width = 70, char fg = '#', char bg = '-');

    ~ProgressBar(); 

    void update(const size_t count = 1);

    void draw();

    void close();

private:
    int                     total_;
    std::string             desc_;
    char                    fg_;
    char                    bg_;
    size_t                  status_;
    std::ostream&           stream_;
    size_t                  width_;
    bool                    closed_;

};

} // namespace cli


#endif  // _CLI_H_
