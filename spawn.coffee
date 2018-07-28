ps = require 'child_process'
shm = require '.'
data = null
workers = []

#Number of child processes
CHILDREN = 16
running = CHILDREN
MEM = 800000 # Two hundred thousand 32-bit integers

#Prepare data
prepare = ->
   mem = shm.open "node_shm:spawn.coffee", MEM
   data = new Int32Array mem
   console.log "#{mem.byteOffset} #{mem.byteLength} #{data.length}"
   N = 1000
   i = 0
   n = 0
   while i < data.length
      n = 0 if n is N
      data[i] = n
      ++i
      ++n
  console.log data

#Start processes
start = ->
 console.log 'start'
 console.time 'count'
 for w in workers
  w.stdin.write 'start'

#Finished process
done = ->
 running--
 if running is 0
  console.timeEnd 'count'

#Spawn process
spawn = (n) ->
 w = ps.spawn 'coffee', ['child.coffee']
 console.time "worker#{n}"

 w.stdout.on 'data', (msg) ->
  console.log "#{n}: #{msg}"
  done()

 w.stderr.on 'data', (msg) ->
  console.log "ERR: #{n}: #{msg}"
  console.timeEnd "worker#{n}"

 w.on 'close', (status) ->
  console.log "EXIT: #{n}: #{status}"
  console.timeEnd "worker#{n}"

 workers.push w

#############

prepare()
for n in [0...CHILDREN]
 spawn n

#Wait for processes to initialize
# setTimeout start, 100
start()
