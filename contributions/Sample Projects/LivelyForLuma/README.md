[![default](https://github.com/user-attachments/assets/1d038ff8-4685-43ee-832b-8ebf9e7375cd)](https://ethglobal.com/showcase/lively-for-luma-du8kj)

More info: https://ethglobal.com/showcase/lively-for-luma-du8kj

## JUST ONE MORE SIDE EVENT BRO.

As chronic crypto conference attendees and hackers, we all know the Luma side event pain too well. Our calendars are way too cluttered, there are 5 bajillion side events happening at the same time, and we’re probably spending more time trying to decide what event to go to than actually enjoying the event itself. There’s no easy way to put it - the Luma side event experience is COOKED.

At Devcon 2024, there were 928 side events. There are so many events happening at the same time, how is one supposed to decide which to go to? Sometimes good events are too far and the traffic is abominable. Sometimes we’re just looking for good food or swag but maybe they ran out already. Sometimes the venue is way too packed and other times it’s completely dead. Sometimes all we want are good networking vibes but it’s mainly the company hosting doing a massive product shill. And maybe sometimes we actually want to do what we’re supposed to be here for and listen in on a good technical talk or panel lineup. 

But who’s gonna let you in on this alpha?

Introducing Lively on Luma: real-time ratings on which Luma side events are poppin’ or floppin’. 

On our app, users can scroll around a map and tap on different pins that show the different Luma events that they’re registered for. For events they’re at or have already popped by that day (if Luma QR code has already been scanned indicating they were checked in), they’ll be able to rate them (upvote or downvote) based on different factors: overall, food, fun, swag, networking, and technical. To decide on what event to go to, they can scope around the different pins and see everyone’s combined ratings for that event. This makes it a personalized experience, as different people might prioritize different things at side events (e.g. good food or good talks).

Why blockchain? By having this on-chain, we ensure that all ratings are valid and given by humans (not bots) and that there is no centralized power that can pay to promote their own event (like how sponsored ads are pushed to the top on Google).

We wanted to build this hackathon project because this is a real problem that we and all of our friends can relate to, and give back to the web3 ecosystem to help make everyone’s conference experience better.

For next steps, we want to be able to add an add friends feature, so that you’ll be able to see which events your friends are currently at and meet up with them. Users will also be incentivized with on-chain reputation and gamification where the more events you rate, the more achievements you unlock, and we’ll even generate a Spotify wrapped type video for you with your preference on the 5 factors and most liked Luma events of the year.

_______

Our project is real-time and is constantly emitting and receiving information. This requires a low cost, high throughput blockchain, so we chose to build on an L2. We started off with building on Arbitrum, which was the perfect choice as we could use Stylus so that we could code in Rust while still making everything EVM-compatible. We also deployed this on Zircuit. 

The frontend was built with Chakra UI, Next.js, and React. We also used the World Mini-App Kit to verify that each rating is provided by a real human (anti-sybil mechanism) and Dynamic for seamless user onboarding so they can login with their Luma email instead of having to connect their wallet to be on-chain. We also integrated Blockscout which makes it easy to debug and track transactions.

## Deploy Solidity Contracts (EVM)
1. `cd ./solidity`
2. Set `RPC_URL` and `PRIVATE_KEY` in `.env` file
3. Run `forge script script/Lively.s.sol:DeployLively --rpc-url $RPC_URL --private-key $PRIVATE_KEY --etherscan-api-key $ETHERSCAN_API_KEY --verify --broadcast`

## Deploy Stylus Contracts (Arbitrum)
TODO

## Arbitrum Deployment
https://sepolia.arbiscan.io/address/0x2B911C14C94cD3628FA6312Da70Fa706284C631B

## Zircuit Deployment
https://explorer.testnet.zircuit.com/address/0xf83e6AF69B226d9446fB8C17CA9f258b91F0202D

## OP Deploymemt
https://optimism-sepolia.blockscout.com/address/0x213eba1702aD8B084F56aF63603dFf67D1D4C9c0
