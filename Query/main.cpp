#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <functional>
#include <boost/regex.hpp>

#include "StellarCartography/SpatialIndex.h"

using namespace StellarCartography;

SpatialIndex loadIndex()
{
    SpatialIndex result;
    std::ifstream is("/home/princet/Coordinates.csv");

    std::string line;
    while (std::getline(is, line).good())
    {
        boost::trim(line);
        std::vector<std::string> strs;
        boost::split(strs, line, boost::is_any_of(","));
        
        result.insert(
            {
                strs[0],
                {
                    boost::lexical_cast<double>(strs[1]),
                    boost::lexical_cast<double>(strs[2]),
                    boost::lexical_cast<double>(strs[3]),
                }
            }
        );
    }

    return result;
}

typedef std::vector<std::string> ArgList;
typedef std::function<void (ArgList)> QueryFcn;
typedef std::unordered_map<std::string, QueryFcn> CommandTable;


void processQuery(const CommandTable& c, const std::string& cmd)
{
    boost::regex split_regex("(^|\\s+)('.*?'|[^\\s]+)");
    std::vector<std::string> args;

    std::transform(
        boost::sregex_iterator(cmd.begin(), cmd.end(), split_regex),
        boost::sregex_iterator(),
        std::back_inserter(args),
        [](boost::smatch m) 
        { 
            return boost::trim_copy_if(m.str(), boost::is_any_of(" \t'")); 
        }
    );
   
    if (args.size() == 0) return;

    auto it = c.find(args[0]);
    if (it == c.end())
    {
        std::cout << "Unknown command: " << args[0] << std::endl;
    }

    it->second(args);
}

int main(int argc, char *argv[])
{
    using namespace std;
    using namespace boost;
    SpatialIndex g = loadIndex();

    CommandTable cmds 
    {
        { "nearest", [&g](ArgList args) 
            {
                Star from = g.getStar(args[1]);
                Star to = 
                    g.nearestNeighbor(from, lexical_cast<double>(args[2]));
                cout << "Neighbor: " << to.getName() 
                     << "Distance: " 
                     << from.getCoords().distance(to.getCoords()) 
                     << endl;
            }
        }
    };

    while (true)
    {
        std::cout << "scq $ ";

        std::string cmd;
        if (!std::getline(std::cin, cmd).good())
        {
            std::cout << std::endl;
            return 0;
        }
        try
        {
            processQuery(cmds, cmd);
        }
        catch (const std::exception& ex)
        {
            cout << "Exception: " << ex.what() << std::endl;
        }
    }
}
