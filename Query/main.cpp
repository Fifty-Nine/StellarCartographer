#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <functional>
#include <boost/regex.hpp>
#include <readline/readline.h>
#include <readline/history.h>

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
    if (it != c.end())
    {
        it->second(args);
    }
    else
    {
        std::cout << "Unknown command: " << args[0] << std::endl;
    }
}

bool prompt(std::string& s)
{
    using namespace std;
    char *buf = readline("scq $ ");

    if (!buf) return false;

    s = std::string(buf);
    add_history(buf);
    free(buf);

    return true;
}

int main(int argc, char *argv[])
{
    using namespace std;
    using namespace boost;
    SpatialIndex g = loadIndex();

    CommandTable cmds 
    {
        { 
            "nearest", 
            [&g](ArgList args) 
            {
                Star from = g.getStar(args[1]);
                Star to = 
                    g.nearestNeighbor(from, lexical_cast<double>(args[2]));
                cout << "Neighbor: " << to.getName() 
                     << " Distance: " 
                     << from.getCoords().distance(to.getCoords()) 
                     << endl;
            }
        },
        { 
            "neighbors", 
            [&g](ArgList args)
            {
                Star from = g.getStar(args[1]);
                double t = lexical_cast<double>(args[2]);
                for (auto n : g.neighbors(from, t))
                {
                    cout << "Neighbor: " << n.getName()
                         << " Distance: " 
                         << from.getCoords().distance(n.getCoords()) 
                         << endl;
                }
            }
        },
        {
            "path",
            [&g](ArgList a)
            {
                Star from = g.getStar(a[1]);
                Star to = g.getStar(a[2]);
                double t = lexical_cast<double>(a[3]);

                for (auto u : g.path(from, to, t))
                {
                    cout << u.getName() << " " 
                         << from.getCoords().distance(u.getCoords())
                         << endl;
                    from = u;
                }
            }
        },
        {
            "reachable",
            [&g](ArgList a)
            {
                Star from = g.getStar(a[1]);
                double t = lexical_cast<double>(a[2]);

                for (auto v : g.reachable(from, t))
                {
                    cout << v.getName() << std::endl;
                }
            }
        },
        {
            "connected",
            [&g](ArgList a)
            {
                double t = lexical_cast<double>(a[1]);

                for (auto cc : g.connectedComponents(t))
                {
                    cout << "Component:" << endl;
                    for (auto v : cc)
                    {
                        cout << "\t" << v.getName() << endl;
                    }
                }
            }
        }
    };

    while (true)
    {
        std::string cmd;
        if (!prompt(cmd))
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
