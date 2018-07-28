shm = require '.'

MEM = 800000 # Two hundred thousand 32-bit integers
mem = shm.open "node_shm:spawn.coffee", MEM
a = new Int32Array mem

start = ->
 c = 0
 k = 0
 while k < 1000
  t = 0
  i = 0
  while i < a.length
   if a[i] is k
    ++t
   ++i

  if t isnt 200
   throw new Error "Counting error k=#{k} t=#{t} #{a.length} #{mem.byteOffset} #{mem.byteLength}"
  c += t
  ++k

 console.log c
 process.exit()


process.stdin.on 'data', (msg) ->
 console.log 'started'
 console.time 'count'
 start()
 console.log( console.timeEnd 'count')


