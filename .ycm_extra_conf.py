def FlagsForFile(f):
  result = [
    '-Wall',
    '-Wextra',
    '-std=c++11',
    '-I.',
    '-I..',
    '-I./Contrib/src/pairs_iterator',
    '-I../Contrib/src/pairs_iterator',
    '-xc++',
    ]
  return { 
      'flags': result,
      'do_cache' : True
    }
