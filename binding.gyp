{
  "targets": [
  {
    "target_name": "shm_addon",
      "sources": ["shm_addon.cpp", "util.cpp", "zmalloc.cpp", "shmem.cpp"],
      "libraries": [
        "-lc",
        "-ldl"
      ],
      "cflags": [
        "-std=c99 -pedantic -O2 -Wall -W"
      ],
      "include_dirs" : [
        "<!(node -e \"require('nan')\")"
      ]}]
}
