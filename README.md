# Simulation and Analysis of BGP Policies in ns-3

## Overview
This project simulates BGP (Border Gateway Protocol) policies and routing behavior using the ns-3 network simulator. It investigates connectivity scenarios involving IIT Delhi’s autonomous system and how routing policies affect traffic paths and network performance.

## Features
- Baseline simulation with simple linear topology (AS1–AS2–AS3)
- Enhanced topology with direct high-bandwidth link (AS1–AS3)
- Simulation of policy-driven routing mimicking BGP’s Local Preference
- Analysis of asymmetric routing scenarios
- Real-world BGP connectivity research included

## Project File Structure

| Folder/File             | Description                                    |
|------------------------|------------------------------------------------|
| `Final report.pdf`      | Complete project report in PDF format           |
| `IPV4 graph.png`        | Graph showing IPv4 traffic analysis             |
| `IPV6 graph.png`        | Graph showing IPv6 traffic analysis             |
| `README.md`             | Project overview and documentation               |
| `bgp.cc`                | Simulation code for baseline BGP scenario        |
| `bgp2.cc`               | Simulation code for enhanced direct link scenario |
| `bgp3.cc`               | Simulation code for policy preference scenario    |
| `bgp_1_animation.xml`   | Animation results file for baseline simulation    |
| `bgp_1_results.xml`     | Simulation results XML for baseline scenario      |
| `bgp_2_animation.xml`   | Animation results file for direct link scenario   |
| `bgp_2_results.xml`     | Simulation results XML for direct link             |
| `bgp_3_animation.xml`   | Animation results file for policy preference scenario |
| `bgp_3_results.xml`     | Simulation results XML for policy preference         |
| `log-partA.png`         | Log output screenshot for baseline simulation      |
| `log-partB.png`         | Log output screenshot for direct link simulation   |
| `log-partC.png`         | Log output screenshot for policy preference scenario |


## Getting Started
To run the simulations, you need to have ns-3 installed. Follow the official ns-3 documentation to set up the environment: [nsnam.org/documentation](https://www.nsnam.org/documentation/)

### Running Simulations
1. Navigate to the simulation directory.
2. Run the simulation scripts (e.g., `./waf --run my_simulation`).
3. Analyze the generated log outputs and XML files in the `xml/` folder.

## Usage
Use this project to:
- Understand the effect of network topology changes on BGP routing
- Observe routing policy impact on path selection and traffic flow
- Experiment with simulating Local Preference policies via static routes
