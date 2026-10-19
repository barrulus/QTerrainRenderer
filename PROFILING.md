# Optional Tracy profiling

Configure with `-DQTR_ENABLE_TRACY=ON -DTracy_DIR=/path/to/lib/cmake/Tracy`
to use an installed Tracy client built with `TRACY_ENABLE=ON`. The option defaults
to OFF. The renderer target propagates the definition and client publicly so
consumers see the same class layout. Use matching client/profiler versions.

CPU zones cover background heightmap building, GUI delivery and application,
mesh/texture upload calls, paintGL, 2D/3D scene and UI submission, shadow/depth
passes and backgrounds. Async request zones carry a renderer address and request
number; delivery records whether the result was still wanted. Worker text uses a
captured address value and does not read the widget from the pool thread.

GPU elapsed spans appear as numeric Tracy plots named
`QTR GPU paintGL ms #N renderer=...`, one per OpenGL context lifetime. These are
not native Tracy GPU timeline events. Two timestamp queries bracket paintGL's
scene/UI commands. Each context owns an eight-frame pool; collection reads only
available results, and a full pool skips measurements instead of waiting.
There is no glFinish or GPU busy-wait loop. Capture reconnects discard old-session
results. Timestamp support is checked at runtime; CPU zones remain available if
it is absent. Query cleanup uses the owning current context, or OpenGL's context
cleanup if that context has already disappeared.

Plot timestamps are collection times, not frame submission times. Final samples
can remain pending while a viewer is idle and be discarded at destruction. Logs
report completed, skipped and pending counts. Plot-name strings intentionally
live for the process, including after widget destruction, because Tracy retains
their pointers. There is one small string per profiled context lifetime.

GPU elapsed spans can include command-stream gaps waiting for the CPU. They are
not GPU busy-time, presentation latency, compositor/swap timing or GPU upload
measurements outside paintGL. CPU upload zones measure the driver calls, not GPU
completion. Nested CPU times include children; worker durations cannot simply
be added to the GUI-thread elapsed time.

Hesiod's `profile_viewer` workload exercises data updates, camera motion, 2D rendering and two
secondary viewer lifetimes while an original viewer keeps rendering. A real
desktop/OpenGL context is required for GPU validation; offscreen widget tests
alone are insufficient. See the integration PR for captured validation results.
