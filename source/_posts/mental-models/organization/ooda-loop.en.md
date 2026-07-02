---
title: 'The OODA Loop: Why Speed of Iteration Beats the Perfect Plan'
lang: en
permalink: en/mental-models/ooda-loop/
alternate: /mental-models/ooda-loop/
categories:
  - Mental Models
  - Organization
tags:
  - Decision
  - Military Strategy
  - Iteration
  - Agent
  - Observability
  - Reinforcement Learning
  - LLM
  - Product Development
abbrlink: 3d625747
date: 2026-07-01 10:00:00
---

> **OODA Loop**
> EN: Observe, orient, decide, act, on repeat—the winner is usually not the best plan but the fastest loop.
> 中文：观察—定向—决策—行动，不停循环；赢的往往不是计划最完美的，而是循环转得最快的。

Here's something almost everyone building agents has been burned by, and hardly anyone names out loud: **in a field moving this fast, the winner is almost never the team with the most carefully thought-out plan—it's the team whose observe-adjust-repeat loop spins fastest. However perfect the plan, it expires before you can ship it.**

<!-- more -->

## Why ask this

Let me describe a scene you've probably lived through.

You design a clean agent architecture: plan first, then execute step by step. In the demo it flows beautifully. Then you point it at a real task and it falls apart. It locks in the whole route at step one, and by step three the tool is plainly returning "that file isn't here," "that endpoint is down"—and the agent barrels ahead along its original map anyway, executing a task it should have abandoned three steps ago, straight into failure. You replay the trace, and the galling part is this: it had every piece of information it needed. It just never used any of it to change its mind.

Now a different scene, equally frustrating. Two teams chase the same goal. Team A spends three months building one "thoroughly considered" big release. Team B ships a small change every week against real data, watches what happens, and adjusts. Three months in, Team A's carefully designed plan is often wrong before it even ships—the base model got upgraded, user needs drifted, and Team B found a path Team A never saw coming.

Both stories are the same story: **in an uncertain, constantly moving environment, what decides the outcome isn't how smart any one decision was—it's how fast you can overturn your last decision with new information.** Slow is the original sin.

None of this is new, and none of it is unique to AI. A US Air Force fighter pilot worked it out half a century ago—writing not about software, but about air combat: two planes trying to get on each other's tail.

## Where it comes from

John Boyd, a US Air Force colonel, was a legendary and famously difficult fighter pilot. His nickname was "Forty-Second Boyd"—he'd bet any pilot forty dollars that from a starting position with the other guy already on his tail, he could reverse it and be on theirs within forty seconds. He reportedly never lost the bet.

What made Boyd restless was that he wasn't satisfied with winning; he had to know *why* he won. In the Korean War, the American F-86 flew against the Soviet MiG-15 and racked up a kill ratio of roughly ten to one. Yet on paper the MiG climbed faster, turned tighter, and flew higher—it should have been the better dogfighter. Boyd spent his life chasing that anomaly: how does the "worse" plane win?

The answer he eventually ground down into a thing called OODA—Observe, Orient, Decide, Act—four words wired into a loop that never stops turning. It started in air combat and ended up seeping into US Marine Corps doctrine, business strategy, even startup methodology. It's simple enough to sketch on a napkin, and it's one of the rare models that gets deeper the more you use it.

## The core intuition

OODA's core fits in one sentence: **you're up against a world that keeps changing, and the key to surviving and winning isn't to devise a perfect plan and then run it—it's to quickly "look, understand, move," then immediately go around again based on how the world reacted.**

The four steps, unpacked:

- **Observe.** Pull new information from the environment—radar, an error message, a tool's return value, user feedback, a monitoring curve. What you can't see, you can't respond to.
- **Orient.** Assemble the scraps you observed into a picture of "what is actually going on right now." This is the most important step of the four, and the most neglected. More on it below.
- **Decide.** Given that picture, choose the next move. In essence, form a hypothesis.
- **Act.** Carry the decision out. And acting changes the environment, which produces a new observation—that's how the loop starts turning.

{% mermaid %}
graph LR
  O[Observe] --> R[Orient]
  R --> D[Decide]
  D --> A[Act]
  A -.each act reshapes the world you observe next.-> O
{% endmermaid %}

Notice that dashed line—**every time you act, the world changes, so you have to go back to Observe and look again.** OODA isn't a pipeline you walk from start to finish; it's a loop that never stops. Whoever's loop turns faster holds the initiative.

Boyd's sharpest phrase for this: **"get inside the opponent's OODA loop."** Meaning: if my loop turns faster than yours, then every time you finally make sense of my last move and react to it, I've already moved two or three times. You're forever responding to a situation that no longer exists—you get more and more rattled, and eventually you back yourself into a corner. The winner isn't necessarily smarter; he just keeps the other guy's picture of the world half a beat behind.

And one more thing Boyd hammered on that nearly everyone drops when they retell OODA: **among the four steps, Orient isn't an equal peer—it's the hidden hub of the whole loop.** The priors, the experience, the mental model you bring to Orient directly decide which observations you'll even notice, which decisions you'll make, which actions you'll take. Orient badly, and no amount of observing or speed of acting saves you—you just rush toward the wrong answer faster.

Keep this key handy, because we'll use it again and again: **it's not who has the stronger single move, it's whose loop turns faster—and who's willing to overturn himself mid-loop. And the quality of the loop bottlenecks at Orient.**

## A real-world example

Back to Boyd's puzzle: how does an F-86 that's worse on paper win ten to one? The answer hides in two unglamorous details that have nothing to do with firepower or top speed.

First, the F-86 had a bubble canopy—far better visibility. The MiG's frame blocked the pilot's view. That means the F-86 pilot's **Observe** was stronger: he saw what the other guy was doing earlier and more completely.

Second, and the part Boyd thought mattered most: the F-86 had hydraulic flight controls, so it switched from one maneuver to the next much faster. The MiG's controls were manual. On any single maneuver the MiG might even be crisper—but the moment you had to chain moves together, the F-86 was far smoother.

Put the two together: the F-86 pilot could see faster (Observe) and switch between a string of moves faster (Act), so his whole OODA loop turned faster than his opponent's. Move for move, the MiG might hold its own. But here's how the F-86 fought—it feints, the MiG starts to react, and before that reaction finishes the F-86 has already changed to the next move. The MiG pilot is perpetually answering a situation that vanished three seconds ago, gets more and more confused, and usually makes the fatal mistake first.

That's what "getting inside the opponent's loop" looks like in practice: **not landing one harder blow, but keeping the other guy permanently half a beat behind.**

It sounds like a half-century-old dogfight, but swap "MiG pilot" for "the team that ships one big release a year and is forever chasing the competitor's last move," and the script doesn't change a word.

## Mapping to AI: an agent's main loop is literally an OODA loop

AI is probably the best place to talk about OODA today, because so much of what we build is, at bottom, a system spinning in a loop inside an uncertain environment.

**An agent's main loop is an OODA loop.** This isn't a metaphor; it's a literal overlap. An agent's working cycle is: call a tool and get the result back (Observe) → understand what that result means (Orient) → decide which tool to call next (Decide) → execute that step (Act) → the result becomes the next observation. The ReAct framework you know so well—that Thought → Action → Observation → Thought… cycle—is OODA under a different name. One of its steps is even literally called Observation.

**Whether it will re-orient is the dividing line between a good agent and a bad one.** Back to that frustrating scene from the top. A plan-then-execute agent front-loads all its Orienting and Deciding into one upfront pass, then blindly Acts to the end. The problem: its orientation froze at the moment of step one. By step three the environment has changed—the file isn't there, the endpoint is down, the earlier assumption was wrong—and it charges ahead holding an expired map. That's the MiG pilot "answering a situation that no longer exists," except this time it's your code. A good agent does the opposite: **every time it observes a new result, it re-orients.** Tool threw an error? Re-read the situation on the spot and change the plan. This is exactly why ReAct-style interleaved loops—think a step, do a step, look at a step—tend to beat plans-thought-to-death on real, messy tasks. Not because they're smarter, but because their OODA loop isn't frozen at the start.

**Eval-driven development is also an OODA loop.** Your model/product iteration cycle is: see where evals and production traces are failing (Observe) → figure out *why* they're failing (Orient, the hard step) → pick a fix, a prompt tweak, a new tool, or a fine-tune (Decide) → ship it and re-run the evals (Act). Hidden in here is a number that's life-or-death for a team: **how long does one turn of your loop take?** If your evals take two weeks to run, your OODA cycle is two weeks; the team next door that closes the loop in two hours iterates dozens of times more in a month—they've gotten inside your loop, and they leave you behind.

**Orient is the hardest step and the one most worth investing in—yet almost everyone optimizes something else.** People pile on logging (thickening Observe) and speed up deploys and add tools (making Act faster), but hardly anyone seriously sharpens "understanding what this observation actually means." For an agent, the quality of Orient rides on its priors—the system prompt, what you packed into the context, whether the retrieved knowledge is right. Same error message: a strong-Orient agent reads "ah, a permissions problem, let me switch paths," while a weak-Orient one reads "task failed" and gives up or flails. **This is why context engineering matters so much: you're not feeding the model trivia, you're deciding the raw material for its Orient step.**

So the two puzzles from the opening collapse into one sentence: **being slow and refusing to re-orient are the same way to die—either your loop turns too slowly, or it froze at the start, and both leave you holding an expired map while a faster loop runs you down.**

## The engineer's view: first, measure how long one turn of your loop takes

Enough principle—down to a few concrete moves.

**Go measure your loop time.** From "spot a problem" to "fixed it, shipped it, saw the result," how much wall-clock elapsed? That number—not how strong your model is—is the real ceiling on your iteration speed. Most teams have never measured it, so they've never known which step their bottleneck actually lives in.

**Make every step of the loop cheap.** Observe depends on observability—the logs you didn't emit and the traces you didn't keep are the battlefield you can't see (you can't respond to what you can't observe). Orient depends on how fast you can pin down root cause. Act depends on whether you dare deploy ten times a day and can roll back with one button. Any slow step slows the whole loop.

**Design agents to re-orient.** Don't front-load one grand plan and then execute blindly. Interleave observing and thinking, and especially after every tool return, give the agent a chance to ask "should I change my mind?" Treat "clinging to the original plan" as a bug to guard against, not as stability.

**But also keep it from spinning itself dizzy.** This is the other side of the coin: an agent that scraps everything and re-plans at the faintest bit of noise thrashes in place and never converges. Re-orient often enough to stay tethered to reality, but not so often that you jump at every shadow. Tuning that balance is one of the hardest things in agent engineering.

The stance for this whole section in one line: **stop staring at "make one decision perfect," and start staring at "make the whole loop turn fast and stay sensitive to new information."** The former is the MiG; the latter is the F-86.

## The investment view: you're not judging how strong it is now, but how fast it gets stronger

Pull the lens back to the company level, and OODA shows up again in a new outfit—and it runs exactly counter to the intuition that "whoever has the highest model score today wins."

A company that can ship weekly and learn from real users will systematically get inside the loop of a company that moves once a quarter. By the time the big company finally reacts to your last feature, you've shipped three more—it's forever chasing your previous move, like that increasingly rattled MiG pilot. The real moat in competition often isn't how strong any one feature is; it's that **your iteration cadence is an order of magnitude faster than the competitor's.**

Looping back to building AI, the lesson here is unusually direct: **when you're judging whether an AI system—or even an AI company—is worth betting on, don't fixate on how strong its model is today. That's a snapshot, and it expires.** What you should actually watch is its OODA cycle: how often it ships, how fast it learns from production, how quickly it recovers when something breaks. In a field where the base models turn over every few months, loop speed is the compounding interest and today's lead is only the principal. Don't bet a marathon on a single snapshot.

## The life view: you learn everything through this loop

This machinery doesn't stay in the server room either.

You learn anything real—coding, an instrument, cooking, a foreign language—not by perfecting the theory before you begin, but through this loop: try it (Act), see how it went (Observe), work out what's off (Orient), adjust (Decide), try again. The faster the feedback and the tighter the loop, the faster you grow. The person who gets in the game early and keeps eating feedback almost always beats the one clutching a perfect plan who never quite starts. It's why "ship an ugly version that runs" usually beats "hold a perfect design in your head"—not because the ugly version is good, but because it got your loop turning.

Think this through, because it's lethal for AI product design: **a good AI tool is, at bottom, one that turns the user's OODA loop faster**—see results sooner, understand why sooner, try the next version sooner. The real value of an AI coding assistant isn't just typing for you; it's compressing your "write a version, read the error, fix a version" loop from minutes to seconds. So when you build one of these, keep asking yourself: **am I actually making the user's loop turn faster, or did I just bolt a flashy feature onto one step and slow the whole loop down?** The answer usually decides whether the feature gets loved or uninstalled.

## Common misconceptions

The first and most common: reading OODA as "just be fast, speed conquers all." **Not quite.** OODA means the whole loop is fast, and especially that **re-orienting** is fast—it's speed of *learning*, not speed of *hands*. If your Orient is wrong, turning faster just makes you rush toward the wrong answer more confidently and more efficiently. A broken-Orient agent looping frantically only winds itself into a dead end faster. Speed only means something when it's built on a willingness to change your mind.

Second: thinking OODA is a tidy four-step pipeline you march through. It isn't. The real diagram Boyd drew is far messier than the clean four-box loop—Orient is a big hub in the middle wired to everything, with feedback lines hanging off both ends. In practice experts skip steps: their Orient is so developed that a glance (Observe) flows almost straight into a move (Act), with Decide short-circuited by muscle memory. OODA is a continuous flow, not four boxes you clock in and out of.

Third: assuming "faster loop" is always good. It isn't. Speed only pays when the environment changes and the opponent moves; and it comes with a precondition—that each of your actions is cheap enough and reversible. That one gets its own section below.

## When it doesn't apply

Knowing when a model **fails** is worth more than memorizing the model. OODA holds on three legs: a changing environment, cheap and reversible actions, and a roughly correct orientation. Flip those, and you get the cases where it doesn't hold:

**Your orientation is wrong.** This is the most dangerous case. If Orient is off, the loop won't save you—it accelerates the crash, because you're pushing wrong decisions out faster. An agent hallucinating its world model runs further off course the faster it loops. Here the move is to stop and fix Orient, not to turn faster.

**The environment is stable and the problem is well-defined.** OODA is built for a moving opponent and a shifting situation. If the problem is fixed and the boundaries are clear—a deterministic math problem, a purely deterministic pipeline—a single carefully-reasoned plan often beats frantic trial and error. Iteration speed is valuable *because* there's uncertainty and change; strip those out and it isn't.

**Each action is expensive or irreversible.** This is the true boundary on "quit talking and just iterate." OODA assumes your actions are cheap and reversible—a pilot can try a maneuver, you can ship and roll back. But if every "action" burns millions (a large pre-training run) or simply can't be undone (a one-way-door decision), you can't trial-and-error your way through by "taking a few more turns"; you have to front-load the thinking. **Cheap and reversible → lean on the loop; expensive and irreversible → lean on deliberation.** This dovetails with the point above: in an agent, any irreversible operation (dropping a database, sending money, messaging the outside world) should be pulled out of the fast loop and gated behind a human confirmation.

The test is that same key, run backwards: **"is my loop turning too slowly, or is my orientation just wrong? Does the environment actually change? Can I undo this move?"** Answer those three, and you'll know whether to hit the gas (turn the loop faster) or hit the brakes (fix Orient first, or think it through before you move).

## Related models

OODA isn't an island; it hangs in a web. Here are some neighbors on that web (articles coming as I write them):

- [Bayesian Thinking](/en/mental-models/bayesian-thinking/): the Orient step is, at bottom, Bayesian updating—revising your read of the world as new evidence arrives. The agent that won't re-orient is the person who won't update their prior.
- [Goodhart's Law](/en/mental-models/goodharts-law/): your whole loop is built on top of the metric you Observe. If that metric has been Goodharted, you're orienting hard on a contaminated observation—and however fast the loop turns, you're just being wrong faster.
- [Second-order Effect](/en/mental-models/second-order-effect/): Act changes the environment, and the changed environment becomes your next Observe—OODA is itself a system full of second-order effects, where every action reshapes the situation you'll face next.
- [PDCA](/en/mental-models/pdca/): Plan–Do–Check–Act, OODA's steady-state cousin from manufacturing and quality control. One leans toward combat, tempo, and seizing initiative; the other toward improvement, stability, and repeatability—both members of the same closed-loop-iteration family.

## The one line to remember

> EN: The world keeps moving, so don't fall in love with your plan—the winner is never the one with the most perfect plan, but the one whose observe-adjust-repeat loop turns fastest and who's willing to overturn it midway.
> 中文：世界一直在变，所以别爱上你的计划——赢的从来不是那个方案最完美的人，是那个“看一眼、改一下、再来一遍”的循环转得最快、还肯在半路推翻自己的人。
