# Simulation and Analysis of BGP Policies in ns-3

## Overview
This project simulates BGP (Border Gateway Protocol) policies and routing behavior using the ns-3 network simulator. It investigates connectivity scenarios involving IIT Delhi’s autonomous system and how routing policies affect traffic paths and network performance.

## Features
- Baseline simulation with simple linear topology (AS1–AS2–AS3)
- Enhanced topology with direct high-bandwidth link (AS1–AS3)
- Simulation of policy-driven routing mimicking BGP’s Local Preference
- Analysis of asymmetric routing scenarios
- Real-world BGP connectivity research included

## Folder Structure
- `figures/`: Screenshots related to BGP connectivity and simulation logs
- `xml/`: ns-3 generated XML files for simulation and animation results
- `src/`: Source code and simulation scripts (optional if applicable)

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
