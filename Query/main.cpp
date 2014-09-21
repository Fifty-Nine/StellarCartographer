#include <boost/algorithm/string.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iostream>
#include <functional>
#include <boost/regex.hpp>
#include <readline/readline.h>
#include <readline/history.h>

#include "StellarCartography/Algorithms.h"
#include "StellarCartography/StarMap.h"

using namespace StellarCartography;

std::list<std::string> star_names;
StarMap g;

void loadIndex()
{
    std::vector<Star> stars;
    std::ifstream is("/home/princet/Coordinates.csv");

    std::string line;
    while (std::getline(is, line).good())
    {
        boost::trim(line);
        std::vector<std::string> strs;
        boost::split(strs, line, boost::is_any_of(","));
        
        stars.push_back(
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

    g = StarMap(stars.begin(), stars.end());
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

typedef std::pair<Coordinate, double> Sample;
template<>
Sample getArg(ArgList args, size_t idx)
{
    std::string name = getArg(args, idx);
    double range = getArg<double>(args, idx + 1);

    return Sample(g.getStar(name).getCoords(), range);
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
    },
    {
        "trilaterate",
        [](ArgList a)
        {
            std::list<Sample> samples;

            for (size_t i = 1; i < a.size(); i += 2)
            {
                samples.emplace_back(getArg<Sample>(a, i));
            }
            
            auto q = 
                StellarCartography::trilaterate(samples.begin(), samples.end());
            cout << q.x() << ", " << q.y() << ", " << q.z() << endl;
        }
    },
    {
        "coordinates",
        [](ArgList a)
        {
            auto p = g.getStar(getArg(a, 1)).getCoords();
            cout << "(" << p.x() << ", " << p.y() << ", " << p.z() << ")" << endl;
        }
    },
    {
        "distance",
        [](ArgList a)
        {
            auto p1 = g.getStar(getArg(a, 1)).getCoords();
            auto p2 = g.getStar(getArg(a, 2)).getCoords();

            cout << p1.distance(p2) << endl;
        }
    },
    {
        "map",
        [](ArgList a)
        {
            auto d = getArg<double>(a, 1);
            Coordinate com = g.centerOfMass();
            double extent = g.extent();
            double scale = 10.0 / extent;

            auto edge_writer = [](std::ostream& os, const Jump& edge)
            {
                os << "[" 
                   << "xlabel=\"" << fixed << setprecision(2) << edge.weight() << "\"" 
                   << "]";
            };

            auto make_label = [](const Star& star)
            {
                Coordinate c = star.getCoords();
                ostringstream ss;
                ss.precision(2);
                ss << fixed;
                ss << "<table border=\"0\" cellborder=\"0\">";
                ss << "<tr><td colspan=\"2\">" << star.getName() << "</td></tr>";
                ss << "<tr><td>X</td><td>" << c.x() << "</td></tr>";
                ss << "<tr><td>Y</td><td>" << c.y() << "</td></tr>";
                ss << "<tr><td>Z</td><td>" << c.z() << "</td></tr>";
                ss << "</table>";
                return ss.str();
            };

            auto vertex_writer = [com, scale, &make_label](
                std::ostream& os, const Star& s)
            {
                Coordinate c = s.getCoords();
                c = { (c.x() - com.x()) * scale, (c.y() - com.y()) * scale, 0.0 };
                os << "[" 
                   << "fillcolor=\"white\" "
                   << "label=<" << make_label(s) << "> "
                   << "shape=box "
                   << "style=\"rounded\" "
                   << "]";
            };

            auto graph_writer = [](std::ostream& os)
            {
                os << "overlap=scale" << endl;
                os << "node [style=filled]" << endl;
            };

            boost::write_graphviz(
                cout, g.byDistance(d), vertex_writer, edge_writer, graph_writer
            );
        }
    }
};

std::string strip_comments(const std::string& s)
{
    bool in_quote = false;
    std::string result;
    for (char c : s)
    {
        if (c == '"' || c == '\'') in_quote = !in_quote;
        if (c == '#' && !in_quote) break;
        result.push_back(c);
    }
    return result;
}

std::vector<std::string> split_args(const std::string& s)
{
    boost::regex split_regex("(^|\\s+)('.*?'|\".*?\"|[^\\s]+)");
    std::vector<std::string> args;

    std::transform(
        boost::sregex_iterator(s.begin(), s.end(), split_regex),
        boost::sregex_iterator(),
        std::back_inserter(args),
        [](boost::smatch m) 
        { 
            return boost::trim_copy_if(m.str(), boost::is_any_of(" \t'\"")); 
        }
    );
    return args;
}

void processQuery(const CommandTable& c, const std::string& cmd)
{
    std::vector<std::string> args = split_args(strip_comments(cmd));
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

    if (isatty(fileno(stdin)))
    {
        rl_completer_quote_characters = "'\"";
        rl_filename_quote_characters = " ";
        rl_filename_quoting_desired = 1;
        rl_attempted_completion_function = &completer;

        char *buf = readline("scq$ ");

        if (!buf) 
        {
            cout << endl;
            return false;
        }

        s = std::string(buf);
        add_history(buf);
        free(buf);
    
        return true;
    }
    return std::getline(cin, s);
}

void redirect(const char *file)
{
    FILE* r = freopen(file, "r", stdin);

    if (!r)
    {
        std::ostringstream ss;
        ss << "Failed to redirect stdin: " << strerror(errno);
        throw std::runtime_error(ss.str());
    }
}

int main(int argc, char *argv[])
{
    using namespace std;
    using namespace boost;

    loadIndex();

    if (argc > 2)
    {
        std::cerr << "Too many command line arguments.";
        return 1;
    }
    if (argc == 2)
    {
        try
        {
            redirect(argv[1]);
        } 
        catch (const std::exception& ex)
        {
            cerr << ex.what() << endl;
        }
    }

    while (true)
    {
        std::string cmd;
        if (!prompt(cmd))
        {
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
