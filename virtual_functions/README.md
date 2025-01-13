# Profiling virtual function calls

Please [find the blog here](https://open.substack.com/pub/sartech/p/snails-tale-of-virtual-function-calls?r=1thl9t&utm_campaign=post&utm_medium=web&showWelcomeOnShare=true)

## Compiling instructions
### Inline vector
`g++-14 -o main main.cpp profiler.cpp -O3 -DINLINE`

### No inline vector
`g++-14 -o main main.cpp profiler.cpp -O3`

### Virtual Vector Inline
`g++-14 -o main main.cpp profiler.cpp -O3 -DINLINE -DVIRTUAL -fno-devirtualize`

### Virtual Vector No Inline
`g++-14 -o main main.cpp profiler.cpp -O3 -DVIRTUAL -fno-devirtualize`

### Virtual Vector Inline Random Object
`g++-14 -o main main.cpp profiler.cpp -O3 -DINLINE -DVIRTUAL -DRANDOM -fno-devirtualize`

### Virtual Vector Inline Multi Object in Pattern (A-B-A-B)
`g++-14 -o main main.cpp profiler.cpp -O3 -DINLINE -DVIRTUAL -DRANDOM -DPATTERN -fno-devirtualize`

### Virtual Vector No Inline Random Object
`g++-14 -o main main.cpp profiler.cpp -O3 -DVIRTUAL -DRANDOM -fno-devirtualize`

### Virtual Vector No Inline Multi Object in Pattern (A-B-A-B)
`g++-14 -o main main.cpp profiler.cpp -O3 -DVIRTUAL -DRANDOM -DPATTERN -fno-devirtualize`
