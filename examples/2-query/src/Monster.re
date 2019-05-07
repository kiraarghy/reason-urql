open MonsterStyles;
open ReasonUrql;

type maximum = {maximum: int};

type pokemon = {
  id: string,
  number: string,
  height: maximum,
  weight: maximum,
};

/* graphql query for GetPokemon */

module GetPokemon = [%graphql
  {|
  query pokemon($name: String!) {
    pokemon(name: $name) {
        name
        classification
        height {
            maximum
        }
        weight {
            maximum
        }
      image
    }
  }
|}
];
let component = ReasonReact.statelessComponent("Example");

let make = (~pokemon: string, _children) => {
  ...component,
  render: _self => {
    /* We set up the query here as we need access to the pokemon
       value passed in from GetAll */
    let query = GetPokemon.make(~name=pokemon, ())##query;
    let variables = Js.Dict.empty();
    Js.Dict.set(variables, "name", Js.Json.string(pokemon));
    let payload = Js.Json.object_(variables);
    <Query query variables=payload>
      ...{
           ({response}: Query.queryRenderProps(GetPokemon.t)) =>
             <>
               {
                 switch (response) {
                 | Data(data) =>
                   switch (data##pokemon) {
                   | Some(pokemon) =>
                     switch (
                       pokemon##image,
                       pokemon##classification,
                       pokemon##name,
                       pokemon##height,
                       pokemon##weight,
                     ) {
                     | (
                         Some(image),
                         Some(classification),
                         Some(name),
                         Some(height),
                         Some(weight),
                       ) =>
                       <section className=Styles.dexContainer>
                         <div className=Styles.dex>
                           <div className=Styles.dexImageContainer>
                             <img className=Styles.dexImage src=image />
                           </div>
                           <div className=Styles.dexText>
                             <h1 className=Styles.dexTitle>
                               name->ReasonReact.string
                             </h1>
                             <h2 className=Styles.dexSubTitle>
                               classification->ReasonReact.string
                             </h2>
                             {
                               switch (height##maximum, weight##maximum) {
                               | (Some(heightMax), Some(weightMax)) =>
                                 <div className=Styles.dexGrid>
                                   <p>
                                     ("Height: " ++ heightMax)
                                     ->ReasonReact.string
                                   </p>
                                   <p>
                                     ("Weight: " ++ weightMax)
                                     ->ReasonReact.string
                                   </p>
                                 </div>
                               | (_, _) => ReasonReact.null
                               }
                             }
                           </div>
                         </div>
                       </section>
                     | (_, _, _, _, _) => ReasonReact.null
                     }
                   | None => ReasonReact.null
                   }
                 | Fetching => <div> "Loading"->ReasonReact.string </div>
                 | Error(error) => <div> "Error"->ReasonReact.string </div>
                 | NotFound => <div> "Not Found"->ReasonReact.string </div>
                 }
               }
             </>
         }
    </Query>;
  },
};