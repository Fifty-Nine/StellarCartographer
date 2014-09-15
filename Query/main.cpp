#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <functional>
#include <boost/regex.hpp>
#include <readline/readline.h>
#include <readline/history.h>

#include "StellarCartography/SpatialIndex.h"

using namespace StellarCartography;

std::list<std::string> star_names;
SpatialIndex g;

void loadIndex()
{
    std::ifstream is("/home/princet/Coordinates.csv");

    std::string line;
    while (std::getline(is, line).good())
    {
        boost::trim(line);
        std::vector<std::string> strs;
        boost::split(strs, line, boost::is_any_of(","));
        
        g.insert(
            {
                strs[0],
                {
                    boost::lexical_cast<double>(strs[1]),
                    boost::lexical_cast<double>(strs[2]),
                    boost::lexical_cast<double>(strs[3]),
                }
            }
        );
        star_names.push_back(strs[0]);
    }
}

typedef std::vector<std::string> ArgList;
typedef std::function<void (ArgList)> QueryFcn;
typedef std::unordered_map<std::string, QueryFcn> CommandTable;
using namespace std; 
using namespace boost;

template<class T = std::string>
T getArg(ArgList args, size_t idx)
{
    if (idx >= args.size()) throw std::out_of_range("Missing argument");
    return lexical_cast<T>(args[idx]);
}

CommandTable cmds 
{
    { 
        "nearest",
        [](ArgList args) 
        {
            Star from = g.getStar(getArg(args, 1));
            Star to = 
                g.nearestNeighbor(from, getArg<double>(args, 2));
            cout << "Neighbor: " << to.getName() 
                 << " Distance: " 
                 << from.getCoords().distance(to.getCoords()) 
                 << endl;
        }
    },
    { 
        "neighbors", 
        [](ArgList args)
        {
            Star from = g.getStar(getArg(args, 1));
            double t = getArg<double>(args, 2);
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
        [](ArgList a)
        {
            Star from = g.getStar(getArg(a, 1));
            Star to = g.getStar(getArg(a, 2));
            double t = getArg<double>(a, 3);

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
        [](ArgList a)
        {
            Star from = g.getStar(getArg(a, 1));
            double t = getArg<double>(a, 2);

            for (auto v : g.reachable(from, t))
            {
                cout << v.getName() << std::endl;
            }
        }
    },
    {
        "connected",
        [](ArgList a)
        {
            double t = getArg<double>(a, 1);

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

std::vector<std::string> split_args(const std::string& s)
{
    boost::regex split_regex("(^|\\s+)('.*?'|[^\\s]+)");
    std::vector<std::string> args;

    std::transform(
        boost::sregex_iterator(s.begin(), s.end(), split_regex),
        boost::sregex_iterator(),
        std::back_inserter(args),
        [](boost::smatch m) 
        { 
            return boost::trim_copy_if(m.str(), boost::is_any_of(" \t'")); 
        }
    );
    return args;
}

void processQuery(const CommandTable& c, const std::string& cmd)
{
    std::vector<std::string> args = split_args(cmd);
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

char *cmd_generator(const char *text, int state)
{
    static CommandTable::const_iterator it;

    if (state == 0)
    {
        it = cmds.begin();
    }

    while (it != cmds.end())
    {
        auto s = it->first;
        ++it;
        if (boost::starts_with(s, text))
        {
            return strdup(s.c_str());
        }
    }

    return NULL;
}

char *star_generator(const char *text, int state)
{
    static std::list<std::string>::const_iterator it;

    if (state == 0)
    {
        it = star_names.begin();
    }

    while (it != star_names.end())
    {
        auto s = *it;
        ++it;
        if (boost::starts_with(s, text))
        {
            if (s.find(' ') != string::npos) s = "'" + s + "'";
            return strdup(s.c_str());
        }
    }
    return NULL;
}

char **completer(const char *text, int start, int end)
{
    auto generator = (start == 0) ? cmd_generator : star_generator;
    return rl_completion_matches(text, generator);
}

bool prompt(std::string& s)
{
    using namespace std;

    rl_attempted_completion_function = &completer;
    rl_bind_key('\t', rl_complete);

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

    loadIndex();

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
